import cv2
from mediapipe.python.solutions import pose as pose
from torch.utils.data import Dataset
import time
import os

# 1. 把影片透過mediapipe轉成landmarks (OK)
def detect_vid( vid_name ):

    videos = cv2.VideoCapture( vid_name ) # get video frames
    # frames = videos.get(cv2.CAP_PROP_FRAME_COUNT)  # total frames
    detect_frame = 0 # frame that can be detected by mediapipe
    landmarks = []

    # 開始偵測
    with pose.Pose() as mp_pose:

        success, frame = videos.read()
        while success:

            frame.flags.writeable = False
            frame = cv2.cvtColor( frame, cv2.COLOR_BGR2RGB )

            result = mp_pose.process( frame )

            if result.pose_landmarks is not None:
                detect_frame = detect_frame + 1
                landmarks.append( result.pose_landmarks.landmark )

            success, frame = videos.read()

    videos.release()

    # print( "total: {}".format( frames ) )
    # print("detect: {}".format( detect_frame ) )
    # print( landmarks[0] )
    return landmarks

# 2. 把偵測到的landmark分割每30 frame一組 (OK)
# 為了讓資料多一點，割法如下: 0-29, 15-44, 30-59....
def cut_landmark( landmarks ):

    head = 0
    tail = 30
    cut_list = []
    size = len( landmarks )
    # print( size )

    while tail <= size:
        cut_list.append( landmarks[ head:tail ] )
        # print( len( landmarks[ head:tail ] ) )
        head = head + 15
        tail = tail + 15

    # print( "list size: {}".format( len( cut_list ) ) )
    return cut_list

# 3. 每一組資料建立一個檔案 (OK)
def write( landmark30, filename, path ):
    # landmark30 shape: 30 * 33 * 4
    wpath = os.path.join( path, filename )  # write path

    with open( wpath, mode="w" ) as writefile:
        for lm in landmark30:
            for i in lm:
                writefile.write( str( i.x ) + ' ' )
                writefile.write( str( i.y ) + ' ' )
                writefile.write( str( i.z ) + ' ' )
                writefile.write( str( i.visibility ) + ' ' )
                writefile.write( '\n' )

            writefile.write( '\n' )

def write_file( lists, class_num, path, f_name ):
    # 取名規則:第一個影片的第一組資料->1_1 第二組:1_2
    # list shape: 6 * 30 * 33 * 4
    # 6組, 30偵, 每偵33個點, 每個點4個數值(x, y, z, visibility)
    data_num = len( lists ) # num of data
    w_dir = os.path.join( path, str( class_num ) ) # directory

    for i in range( data_num ):
        name = str( f_name ) + '_' + str( i ) + ".txt"
        write( landmark30 = lists[i], filename = name, path = w_dir )

# video dataSet (OK)
class MyVideoData( Dataset ):

    # 初始化
    def __init__( self, root_dir, label_dir ):
        self.root_dir = root_dir
        self.label_dir = label_dir
        self.path = os.path.join( self.root_dir, self.label_dir ) # 把兩個路徑加起來
        self.vid_list = os.listdir( self.path ) # 所有影片檔案的List

    # 取得第i個影片和他的label
    def __getitem__( self, idx ):
        vid_name = self.vid_list[idx]
        vid_path = os.path.join( self.path, vid_name ) # 影片位置
        class_name = self.label_dir
        return vid_path, class_name

    def __len__( self ):
        return len( self.vid_list )

if __name__ == '__main__':

    # Get origin data directory 得到22類的資料夾(影片檔)
    root = "workout_vid"
    list_dir = os.listdir( root )

    total_vid = 0  # total vid used
    count = 0 # counter
    classes = 0  # different classes number
    write_path = 'total_vid_data/'

    start = time.time()

    print("start running")
    # for every class( total 22 classes ) 22類所有照片都偵測一遍
    for label_dir in list_dir:

        # read videos
        print("Read videos {}".format( label_dir ) )
        data = MyVideoData( root, label_dir )
        total_vid = total_vid + data.__len__()

        # detect 每一類別的所有影片
        print("Detecting...")
        for vid, label in data:

            landmarks = detect_vid( vid_name = vid ) # 1. 把影片透過mediapipe轉成landmarks
            cut_list = cut_landmark( landmarks ) # 2. 把偵測到的landmark分割每30 frame一組
            write_file( lists = cut_list,
                        class_num = classes,
                        path = write_path,
                        f_name = count ) # 3. 每一組資料建立一個檔案, 依不同類別
            count = count + 1 # 紀錄此類用了幾個影片

        print( "Class {} OK".format( classes ) )
        count = 0 # 下一分類重新計算
        classes = classes + 1

    end = time.time() - start
    print( 'Time: {}'.format( end ) )
    # print( 'Total videos: {}'.format( total_vid ) )
