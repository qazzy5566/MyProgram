import math
import numpy as np
from mediapipe.python.solutions import pose as pose
from mediapipe.python.solutions import drawing_utils as draw
from mediapipe.python.solutions import drawing_styles as draw_style
import time
import cv2 as cv

# calaulate the joint angle
def angle3D(a, b, c):
    v1 = [ (a.x - b.x), (a.y - b.y), (a.z - b.z) ]
    v2 = [ (c.x - b.x), (c.y - b.y), (c.z - b.z) ]
    angle = np.arccos( np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2) ) )
    angle = math.degrees( angle ) # 把弧度改成角度
    return round( angle )

def joint_angle( landmarks ):
    lm11 = landmarks[11] # left shoulder
    lm12 = landmarks[12] # right shoulder
    lm13 = landmarks[13] # left elbow
    lm14 = landmarks[14] # right elbow
    lm15 = landmarks[15] # left wrist
    lm16 = landmarks[16] # right wrist
    lm23 = landmarks[23] # left hip
    lm24 = landmarks[24] # right hip
    lm25 = landmarks[25] # left knee
    lm26 = landmarks[26] # right knee
    lm27 = landmarks[27] # left ankle
    lm28 = landmarks[28] # right ankle

    # 身體跟手臂的角度(左): 13、11、23
    angle1 = angle3D( lm13, lm11, lm23 )
    # 身體跟手臂的角度(右): 14、12、24
    angle2 = angle3D( lm14, lm12, lm24 )
    # 左手手肘: 11、13、15
    angle3 = angle3D( lm11, lm13, lm15 )
    # 右手手肘: 12、14、16
    angle4 = angle3D( lm12, lm14, lm16 )
    # 腰的角度(左): 11、23、25
    angle5 = angle3D( lm11, lm23, lm25 )
    # 腰的角度(右): 12、24、26
    angle6 = angle3D( lm12, lm24, lm26 )
    # 腿的角度(左): 23、25、27
    angle7 = angle3D( lm23, lm25, lm27 )
    # 腿的角度(右): 24、26、28
    angle8 = angle3D( lm24, lm26, lm28 )

    angle_list = [ angle1, angle2, angle3, angle4, angle5, angle6, angle7, angle8 ]
    return angle_list

# 把一張圖片的人體landmark找出來然後算各關節角度

# read a image
img = cv.imread( "Total_Img/2/03.png" )

# resize the image
# img_resize = cv.resize( img, (224, 224) )

with pose.Pose() as mp_pose:

    # calculate time cost
    start_time = time.time()

    # To improve performance, optionally mark the image as not writeable to
    # pass by reference.
    img.flags.writeable = False

    # opencv 讀圖片的格式是BGR，要改成RGB
    img = cv.cvtColor( img, cv.COLOR_BGR2RGB )

    # detect pose
    result = mp_pose.process( img )

    # show result
    img.flags.writeable = True
    img = cv.cvtColor( img, cv.COLOR_RGB2BGR ) # 改回BGR格式
    draw.draw_landmarks(img,
                        result.pose_landmarks,
                        pose.POSE_CONNECTIONS,
                        landmark_drawing_spec=draw_style.get_default_pose_landmarks_style()
                        )

    # 開始算角度
    angle_list = joint_angle( result.pose_world_landmarks.landmark )
    print( angle_list )

    # calculate time cost
    end_time = time.time() - start_time

    cv.imshow('windows', img)
    cv.waitKey(0)
    print( "Time: {}".format( end_time ) )

