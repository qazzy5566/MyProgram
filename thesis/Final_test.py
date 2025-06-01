# test the model
import time
import torch
from torch import nn
import cv2
from mediapipe.python.solutions import pose as pose
from mediapipe.python.solutions import drawing_utils as draw
from mediapipe.python.solutions import drawing_styles as draw_style

# Step 1. read video
# Step 2. mediapipe
# Step 3. lstm model(every 30 frame)
# Step 4. Show result on screen

class LstmModel( nn.Module ):
    def __init__( self, input_size, hidden_size, num_layer = 1, drop_rate = 0, batch_first = False ):
        super( LstmModel, self ).__init__()
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

def lm_to_list( landmark ):

    lm_list = []
    for lm in landmark:
        lm_list.append( [ lm.x, lm.y, lm.z, lm.visibility ] )

    return lm_list

def classify( landmark30, model ):
    # landmark30.shape:(30, 33, 4)
    all_classes = ['barbell biceps curl',
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
    model.eval()
    x = landmark30.reshape(1, 30, 33 * 4).to( device )
    predict = model( x )

    return all_classes[ predict.argmax(1) ]

device = "cuda" if torch.cuda.is_available() else "cpu"
# trained model
model_path = "models/LSTM_model_XXX30.pth"
model = LstmModel(
        input_size = 33 * 4,
        hidden_size = 128,
        batch_first = True
        ).to( device )
model.load_state_dict( torch.load( model_path ) )

# 1.
vid = cv2.VideoCapture( "workout_vid/pull Up/pull up_10.mp4" )
# workout_vid/push-up/push-up_3.mp4
# workout_vid/squat/squat_11.mp4
# workout_vid/plank/plank_2.mp4
# workout_vid/pull Up/pull up_10.mp4
# test/IMG_0114.mp4 ( 115 116 119 )

prevTime = 0
currTime = 0
recognition_time = 0
count = 0
frames = 0
lms = []
predict_result = 'None'
width = int( vid.get( cv2.CAP_PROP_FRAME_WIDTH ) )
height = int( vid.get( cv2.CAP_PROP_FRAME_HEIGHT ) )
fourcc = cv2.VideoWriter_fourcc(*'DIVX')
# out = cv2.VideoWriter( 'output.mov', fourcc, 20.0, ( width,  height ) )

# 2.
with pose.Pose() as mp_pose:

    success, frame = vid.read()
    while success:

        frame.flags.writeable = False
        frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        result = mp_pose.process( frame )
        frames = frames + 1

        # 3.
        if result.pose_landmarks is not None:
            lms.append( lm_to_list( result.pose_landmarks.landmark ) )
            count = count + 1

        if count == 30:
            count = 0
            list_tensor = torch.tensor( lms ).to( torch.float32 )  # 更改格式 to fit the pytorch model
            lms = []
            start = time.time()
            predict_result = classify( list_tensor, model )
            recognition_time = time.time() - start

        # Draw result
        frame.flags.writeable = True
        frame = cv2.cvtColor( frame, cv2.COLOR_RGB2BGR )  # 改回BGR格式
        draw.draw_landmarks(frame,
                            result.pose_landmarks,
                            pose.POSE_CONNECTIONS,
                            landmark_drawing_spec = draw_style.get_default_pose_landmarks_style()
                            )

        # 可以算FPS
        currTime = time.time()
        fps2 = 1 / ( currTime - prevTime )
        prevTime = currTime
        # fps show in screen
        cv2.putText(frame,
                    f'FPS : { int( fps2 ) }',
                    ( 30, 50 ),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1,
                    ( 0, 255, 0 ),
                    3)
        cv2.putText(frame,
                    f'{ predict_result }',
                    ( 100, 100 ),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    3,
                    ( 0, 255, 0 ),
                    3)
        cv2.putText(frame,
                    f'recognition : { recognition_time }',
                    ( 20, 30 ),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1,
                    ( 255, 0, 0 ),
                    3)

        # if frames == 50:
        #     cv2.imwrite( "test4.png", frame )
        # out.write( frame )
        cv2.imshow( 'windows', frame )
        # cv2.waitKey( int(1000 / fps) )
        # press 'q' to stop
        if cv2.waitKey(1) == ord('q'):
            break

        success, frame = vid.read()

print( "recognition time: {}".format( recognition_time ) )
vid.release()
# out.release()