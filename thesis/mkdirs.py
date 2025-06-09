import os

# 建22個資料夾放訓練資料跟測試資料
root = 'total_vid_data/'
for i in range( 22 ):

    path = os.path.join( root, str( i ) )
    os.mkdir( path )