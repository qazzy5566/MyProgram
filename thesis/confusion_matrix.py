# confusion matrix
# last epoch
import os
from sklearn.metrics import confusion_matrix
import seaborn as sn
import pandas as pd
import torch.nn as nn
import torch
import numpy as np
import matplotlib.pyplot as plt
from torch.utils.tensorboard import SummaryWriter
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
from torcheval.metrics import MulticlassPrecision
from torcheval.metrics import MulticlassRecall

def create_confusion_matrix( loader, net ):
    y_pred = [] # save prediction
    y_true = [] # save ground truth
    size = len( dataloader.dataset )
    precision_metric = MulticlassPrecision( num_classes = 22, average = 'macro' )
    recall_metric = MulticlassRecall( num_classes = 22, average = 'macro' )
    accuracy = 0

    # iterate over data
    for inputs, labels in loader:
        inputs = inputs.reshape(-1, 30, 33 * 4).to( device )
        labels = labels.to( device )
        output = net( inputs )  # Feed Network

        accuracy += ( output.argmax(1) == labels ).type(torch.float).sum().item()

        output = ( torch.max( torch.exp( output ), 1 )[1] ).data.cpu().numpy()
        y_pred.extend( output )  # save prediction

        labels = labels.data.cpu().numpy()
        y_true.extend( labels )  # save ground truth

    # Build confusion matrix
    # constant for classes
    classes = ['barbell biceps curl',
                'bench press',
                'chest fly machine',
                'deadlift',
                'decline bench press',
                'hammer curl',
                'hip thrust',
                'incline bench press',
                'lat pulldown',
                'lateral raises',
                'leg extension',
                'leg raises',
                'plank',
                'pull up',
                'push up',
                'romanian deadlift',
                'russian twist',
                'shoulder press',
                'squat',
                't bar row',
                'tricep dips',
                'tricep pushdown']
    y_true_tensor = torch.LongTensor( y_true )
    y_pred_tensor = torch.LongTensor( y_pred )

    precision_metric.update( y_pred_tensor, y_true_tensor )
    recall_metric.update( y_pred_tensor, y_true_tensor )
    accuracy /= size # Accuracy
    precision = precision_metric.compute() # precision
    recall = recall_metric.compute() # recall
    f1_score = ( 2 * precision * recall ) / ( precision + recall ) # F1_score

    print( "Accuracy: {}".format( accuracy ) )
    print( "precision: {}".format( precision ) )
    print( "recall: {}".format( recall ) )
    print( "f1_score: {}".format( f1_score ) )

    cf_matrix = confusion_matrix(y_true, y_pred)
    df_cm = pd.DataFrame(cf_matrix / np.sum(cf_matrix, axis=1)[:, None], index=[i for i in classes],
                         columns=[i for i in classes])
    plt.figure( figsize=( 15, 15 ) )
    return sn.heatmap(df_cm, annot=True).get_figure()

# read txt file OK
def read_one_data( path ):
    temp_list = []
    list_ = []

    with open( path, mode="r" ) as reader:

        for i in range( 30 ): # 30偵

            for j in range( 33 ): # 33個landmark
                line = reader.readline()
                s = line.split()
                s[0] = np.float32(s[0]) # lm.x
                s[1] = np.float32(s[1]) # lm.y
                s[2] = np.float32(s[2]) # lm.z
                s[3] = np.float32(s[3]) # lm.visibility
                temp_list.append( s )

            reader.readline() # 跳過空格
            # list_tensor = torch.tensor( temp_list ).to( torch.float32 ) # 更改格式
            # reshape_tensor = torch.reshape( list_tensor, ( 1, 18, 3 ) )
            list_.append( temp_list )
            temp_list = [] # temp 清空
        list_tensor = torch.tensor( list_ ).to( torch.float32 ) # 更改格式

    return list_tensor

# All landmark data
class LmData( Dataset ):

    # init
    def __init__( self, root_dir, label_dir ):
        self.root_dir = root_dir
        self.label_dir = label_dir
        self.path = os.path.join( self.root_dir, self.label_dir )
        self.file_list = os.listdir( self.path ) # data list

    def __getitem__( self, idx ):
        filename = self.file_list[ idx ]
        file_path = os.path.join( self.path, filename )
        landmark = read_one_data( file_path )
        label = torch.tensor( int( self.label_dir ) ).to( torch.float32 )
        return landmark, label

    def __len__(self):
        return len( self.file_list )

class Workout22( Dataset ):

    # init
    def __init__( self, root_dir ):
        self.root_dir = root_dir
        self.label_dir_list = os.listdir( root_dir )
        for i, label_dir in enumerate( self.label_dir_list ):
            temp_data = LmData( self.root_dir, label_dir )
            if i == 0:
                self.data = temp_data
            else:
                self.data = self.data + temp_data


    def __getitem__( self, idx ):
        return self.data[ idx ]

    def __len__(self):
        return self.data.__len__()

class LSTM_model( nn.Module ):
    def __init__( self, input_size, hidden_size, num_layer = 1, drop_rate = 0, batch_first = False ):
        super( LSTM_model, self ).__init__()
        self.batch_first = batch_first
        self.num_layers = num_layer
        self.hidden_size = hidden_size
        self.lstm = nn.LSTM(
            input_size = input_size, # 33 * 4
            hidden_size = hidden_size, # 128
            num_layers = num_layer, # 1
            dropout = drop_rate, # 0
            batch_first = batch_first # True
        )

        self.linear = nn.Linear( hidden_size, 22 )

    def forward( self, x ):
        h_0 = torch.zeros( self.num_layers, x.size( 0 ), self.hidden_size ).to( device )
        c_0 = torch.zeros( self.num_layers, x.size( 0 ), self.hidden_size ).to( device )
        out, ( h_n, c_n ) = self.lstm( x, ( h_0, c_0 ) )
        # out.shape: ( batch, seq, hidden )

        # Decode the hidden state of the last time step
        if self.batch_first is False:
            out = out[ -1 ]
        else: # batch first = true
            out = out[ :, -1, : ]

        output = self.linear( out )
        # ( Batch, 22 )

        return output

device = "cuda" if torch.cuda.is_available() else "cpu"

model_path = "models/LSTM_model_XXX30.pth"
model = LSTM_model(
        input_size = 33 * 4,
        hidden_size = 128,
        batch_first = True
        ).to( device )
model.load_state_dict( torch.load( model_path ) )

data = Workout22( "total_vid_data/" )

train_data, test_data = torch.utils.data.random_split( data, [ 0.8, 0.2 ] )

dataloader = DataLoader( dataset = data, batch_size = 64, shuffle = True )

writer = SummaryWriter('tensorboard')

writer.add_figure( "Confusion Matrix", create_confusion_matrix( dataloader, model ), 1 )

writer.close()