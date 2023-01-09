import cv2
from mediapipe.python.solutions import pose as pose
from mediapipe.python.solutions import drawing_utils as draw
from mediapipe.python.solutions import drawing_styles as draw_style
import time
# import mediapipe as mp

cam = cv2.VideoCapture(0)
# pose = mp.solutions.pose
# draw = mp.solutions.drawing_utils
# draw_style = mp.solutions.drawing_styles
# help(pose.Pose)

prevTime = 0 # 看畫面是幾偵
currTime = 0

with pose.Pose() as mp_pose:

    while cam.isOpened():
        success, img = cam.read() # return two value, if img is ok, ret = true
        if not success:
            print('Ignoring empty camera frame.') # 相機讀圖片失敗
            continue

        # To improve performance, optionally mark the image as not writeable to
        # pass by reference.
        img.flags.writeable = False

        # opencv 讀圖片的格式是BGR，要改成RGB
        img = cv2.cvtColor( img, cv2.COLOR_BGR2RGB )

        # 偵測圖片
        result = mp_pose.process( img )

        # 把偵測結果在圖片上顯示(用畫的)
        img.flags.writeable = True
        img = cv2.cvtColor( img, cv2.COLOR_RGB2BGR ) # 改回BGR格式
        draw.draw_landmarks( img,
                             result.pose_landmarks,
                             pose.POSE_CONNECTIONS,
                             landmark_drawing_spec = draw_style.get_default_pose_landmarks_style()
                            )

        # Flip the image horizontally for a selfie-view display.
        # 把圖片水平翻轉，就比較像照鏡子那樣
        img = cv2.flip( img, 1 )

        # 可以算FPS
        currTime = time.time()
        fps = 1 / (currTime - prevTime)
        prevTime = currTime
        # fps show in screen
        cv2.putText(img,
                    f'FPS : {int(fps)}',
                    (30, 50),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1,
                    (255, 0, 0),
                    3)

        cv2.imshow( 'Mediapipe Pose', img )

        # press 'esc' to stop
        if cv2.waitKey(5) & 0xFF == 27:
            break

cam.release()

