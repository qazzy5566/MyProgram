# sample frame of the dataset
import cv2
import torchvision.utils
from torch.utils.tensorboard import SummaryWriter
from torchvision import transforms

img1 = "workout_img/barbell biceps curl/barbell biceps curl_100001.jpg"
# img2 = "workout_img/bench press/bench press_100001.jpg"
img3 = "workout_img/chest fly machine/cfm_100001.jpg"
# img4 = "workout_img/deadlift/deadlift_100001.jpg"
img5 = "workout_img/decline bench press/dbp_200001.jpg"
# img6 = "workout_img/hammer curl/hammer curl_100001.jpg"
img7 = "workout_img/hip thrust/hip thrust_100061.jpg"
# img8 = "workout_img/incline bench press/incline bench press_100001.jpg"
img9 = "workout_img/lat pulldown/lat pulldown_900021.jpg"
# img10 = "workout_img/lateral raises/lateral raise_100001.jpg"
img11 = "workout_img/leg extension/leg extension_100031.jpg"
# img12 = "workout_img/leg raises/leg raises_100001.jpg"
img13 = "workout_img/plank/plank_100001.jpg"
# img14 = "workout_img/pull up/pull up_100061.jpg"
img15 = "workout_img/push up/push up_100001.jpg"
# img16 = "workout_img/romanian deadlift/romanian deadlift_100001.jpg"
img17 = "workout_img/russian twist/russian twist_100001.jpg"
# img18 = "workout_img/shoulder press/shoulder press_100011.jpg"
img19 = "workout_img/squat/squat_100041.jpg"
# img20 = "workout_img/t bar row/t bar row_100001.jpg"
img21 = "workout_img/tricep dips/tricep dips_100061.jpg"
# img22 = "workout_img/tricep pushdown/tricep pushdown_100001.jpg"
images = []
images.append( cv2.imread( img1 ) )
# images.append( cv2.imread( img2 ) )
images.append( cv2.imread( img3 ) )
# images.append( cv2.imread( img4 ) )
images.append( cv2.imread( img5 ) )
# images.append( cv2.imread( img6 ) )
images.append( cv2.imread( img7 ) )
# images.append( cv2.imread( img8 ) )
images.append( cv2.imread( img9 ) )
# images.append( cv2.imread( img10 ) )
images.append( cv2.imread( img11 ) )
# images.append( cv2.imread( img12 ) )
images.append( cv2.imread( img13 ) )
# images.append( cv2.imread( img14 ) )
images.append( cv2.imread( img15 ) )
# images.append( cv2.imread( img16 ) )
images.append( cv2.imread( img17 ) )
# images.append( cv2.imread( img18 ) )
images.append( cv2.imread( img19 ) )
# images.append( cv2.imread( img20 ) )
images.append( cv2.imread( img21 ) )
# images.append( cv2.imread( img22 ) )

to_tensor = transforms.ToTensor()
resize512 = transforms.Resize( (512, 512) )

for i in range( 11 ):
    img = cv2.cvtColor(images[i], cv2.COLOR_BGR2RGB)
    temp = to_tensor( img )
    images[i] = resize512.forward( temp )

writer = SummaryWriter( "tensorboard" )

grid = torchvision.utils.make_grid( images )
writer.add_image( 'images', grid, 0 )

writer.close()