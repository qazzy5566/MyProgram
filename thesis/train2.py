import time
import os # dir list
from torch.utils.data import Dataset
from torch.utils.data import DataLoader
import torch
from torch import nn
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import confusion_matrix
import seaborn as sn
import pandas as pd
from torch.utils.tensorboard import SummaryWriter
from torcheval.metrics import MulticlassPrecision
from torcheval.metrics import MulticlassRecall

def train( dataloader, model, loss_fn, optimizer, device ):
    size = len( dataloader.dataset )
    model.train()

    for batch, ( x, y ) in enumerate( dataloader ):
        y = y.type( torch.LongTensor )
        y = y.to( device )
        x = x.reshape( -1, 30, 33 * 4 ).to( device )

        # Compute prediction error
        predict = model( x )
        loss = loss_fn( predict, y )

        # Backpropagation
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        # show progress
        if batch % 50 == 0:
            loss, current = loss.item(), (batch + 1) * len( x )
            print( f"Train loss: {loss:>7f}  [{current:>5d}/{size:>5d}]" )

def test( dataloader, model, loss_fn, device ):
    size = len( dataloader.dataset )
    num_batches = len( dataloader )
    model.eval()
    test_loss, correct = 0, 0
    with torch.no_grad():
        for x, y in dataloader:
            y = y.type( torch.LongTensor )
            y = y.to( device )
            x = x.reshape( -1, 30, 33 * 4 ).to( device )
            predict = model( x )
            test_loss += loss_fn( predict, y ).item()
            correct += ( predict.argmax(1) == y ).type( torch.float ).sum().item()
    test_loss /= num_batches
    correct /= size
    print( f"Accuracy: {(100*correct):>0.1f}%, Avg loss: {test_loss:>8f} \n" )

    return test_loss, correct

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

def create_confusion_matrix( loader, net ):
    y_pred = [] # save prediction
    y_true = [] # save ground truth

    # iterate over data
    for inputs, labels in loader:
        inputs = inputs.reshape(-1, 30, 33 * 4).to( device )
        labels = labels.to( device )
        output = net( inputs )  # Feed Network

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

    cf_matrix = confusion_matrix(y_true, y_pred)
    df_cm = pd.DataFrame(cf_matrix / np.sum(cf_matrix, axis=1)[:, None], index=[i for i in classes],
                         columns=[i for i in classes])
    plt.figure( figsize=( 15, 15 ) )
    return sn.heatmap(df_cm, annot=True).get_figure()

def precision_recall_f1score( loader, net ):
    y_pred = []  # save prediction
    y_true = []  # save ground truth
    size = len( loader.dataset )
    precision_metric = MulticlassPrecision( num_classes = 22, average = 'macro' )
    recall_metric = MulticlassRecall( num_classes = 22, average='macro' )
    accuracy = 0

    # iterate over data
    for inputs, labels in loader:
        inputs = inputs.reshape(-1, 30, 33 * 4).to(device)
        labels = labels.to(device)
        output = net(inputs)  # Feed Network

        accuracy += ( output.argmax(1) == labels ).type(torch.float).sum().item()

        output = (torch.max(torch.exp(output), 1)[1]).data.cpu().numpy()
        y_pred.extend(output)  # save prediction

        labels = labels.data.cpu().numpy()
        y_true.extend( labels )  # save ground truth

    y_true_tensor = torch.LongTensor( y_true )
    y_pred_tensor = torch.LongTensor( y_pred )

    precision_metric.update(y_pred_tensor, y_true_tensor)
    recall_metric.update(y_pred_tensor, y_true_tensor)
    accuracy /= size  # Accuracy
    precision = precision_metric.compute()  # precision
    recall = recall_metric.compute()  # recall
    f1_score = (2 * precision * recall) / (precision + recall)  # F1_score

    print("Accuracy: {}".format(accuracy))
    print("precision: {}".format(precision))
    print("recall: {}".format(recall))
    print("f1_score: {}".format(f1_score))

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

if __name__ == '__main__':

    # all_data.shape = [all_data_len, 2] # [ 資料量, [data, label] ]
    data = Workout22( "total_vid_data/" )

    # print( data[0][0].shape )

    # Get cpu or gpu device for training.
    device = "cuda" if torch.cuda.is_available() else "cpu"
    print( f"Using {device} device" )

    # model parameter
    input_size = 33 * 4
    hidden_size = 128
    num_layer = 1
    # Hyperparameter
    batch_size = 64
    learning_rate = 0.002
    drop_rate = 0.2 # unuse
    weight_decay = 0
    epochs = 30

    # create model or load model
    # num_layer = 1 by default
    model = LSTM_model(
        input_size = input_size,
        hidden_size = hidden_size,
        batch_first = True
    ).to( device )
    # model_path = "models/LSTM_model3-1_30.pth"
    # model.load_state_dict( torch.load( model_path ) )

    # loss function and an optimizer.
    loss_fn = nn.CrossEntropyLoss()
    # optimizer = torch.optim.SGD( model.parameters(), lr = learning_rate, weight_decay = weight_decay )
    optimizer = torch.optim.Adam( model.parameters(), lr = learning_rate )
    scheduler = torch.optim.lr_scheduler.MultiStepLR(
        optimizer = optimizer,
        milestones = [10, 20],
        gamma = 0.5
    )

    # split the data 80% for train, 20% for test
    train_data, test_data = torch.utils.data.random_split( data, [ 0.8, 0.2 ] )
    # print( "{},  {}".format( len( train_data ), len( test_data ) ) ) # [6812, 1702]

    dataloader = DataLoader( dataset = data, batch_size = batch_size, shuffle = True )
    train_dataloader = DataLoader( dataset = train_data, batch_size = batch_size, shuffle = True )
    test_dataloader = DataLoader( dataset = test_data, batch_size = batch_size, shuffle = True )

    # # 看一下資料shape
    # for x, y in train_dataloader:
    #     print(f"Shape of X [N, C, H, W]: {x.shape}") # [64, 30, 33, 4]
    #     print(f"Shape of y: {y.shape} {y.dtype}")
    #     break

    train_loss = []
    train_accuracy = []
    test_loss = []
    test_accuracy = []

    start_time = time.time()
    # print( model )
    # train & test
    for t in range( epochs ):
        print( f"Epoch {t + 1}, lr:{learning_rate}\n-------------------------------" )

        # Training
        train( train_dataloader, model, loss_fn, optimizer, device )

        # validation
        print( "Train Set:" )
        temp_loss, temp_correct = test( train_dataloader, model, loss_fn, device )
        train_loss.append( temp_loss )
        train_accuracy.append( temp_correct )
        print( "Test Set:" )
        temp_loss, temp_correct = test( test_dataloader, model, loss_fn, device )
        test_loss.append( temp_loss )
        test_accuracy.append( temp_correct )

        scheduler.step() # change lr every 10 epoch : 0.002 > 0.001 > 0.0005

        # Save model every 10 epochs
        if ( t + 1 ) % 10 == 0:
            model_name = 'LSTM_NEW' + ".pth"
            torch.save( model.state_dict(), "models/" + model_name )
            print( "Saved PyTorch Model State to {}".format( model_name ) )

    print( "Done!!!!!!!" )

    # calculate time cost
    end_time = time.time() - start_time
    print( "Time: {}".format(end_time) )

    # 紀錄資料
    print( "Train loss:" )
    print( train_loss )
    print( "Train acc:" )
    print( train_accuracy )
    print("Test loss:")
    print( test_loss )
    print( "Test acc:" )
    print( test_accuracy )

    # confusion matrix
    writer = SummaryWriter('tensorboard')
    writer.add_figure("Confusion Matrix", create_confusion_matrix( dataloader, model ), 1)
    writer.close()

    # precision recall f1_score
    print( "train performance metric" )
    precision_recall_f1score( train_dataloader, model )
    print( "test performance metric" )
    precision_recall_f1score( test_dataloader, model )

    fig, ( loss, acc ) = plt.subplots( 2 )
    # loss graph
    loss.plot( list( range( 1, epochs + 1 ) ), train_loss, 'tab:blue', label = "Train" )
    loss.plot( list( range( 1, epochs + 1 ) ), test_loss, 'tab:red', label = "Test" )
    loss.set_title( 'Loss' )
    loss.legend()
    loss.grid()
    loss.set( xlabel = 'Epoch', ylabel = 'Loss' )
    # arr graph
    acc.plot( list( range( 1, epochs + 1 ) ), train_accuracy, 'tab:blue', label = "Train" )
    acc.plot( list( range( 1, epochs + 1 ) ), test_accuracy, 'tab:red', label = "Test" )
    acc.set_title( 'Accuracy' )
    acc.legend()
    acc.grid()
    acc.set( xlabel = 'Epoch', ylabel = 'Accuracy' )

    plt.show()


