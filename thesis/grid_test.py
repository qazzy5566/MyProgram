# sample frame of the dataset
import cv2
import torchvision.utils
from torch.utils.tensorboard import SummaryWriter
from torchvision import transforms

img1 = "test1.png"
img2 = "test2.png"
img3 = "test3.png"
img4 = "test4.png"
images = []
images.append( cv2.imread( img1 ) )
images.append( cv2.imread( img2 ) )
images.append( cv2.imread( img3 ) )
images.append( cv2.imread( img4 ) )

to_tensor = transforms.ToTensor()
resize512 = transforms.Resize( (512, 512) )

for i in range( 4 ):
    img = cv2.cvtColor(images[i], cv2.COLOR_BGR2RGB)
    temp = to_tensor( img )
    images[i] = resize512.forward( temp )

writer = SummaryWriter( "tensorboard" )

grid = torchvision.utils.make_grid( images, 2 )
writer.add_image( 'test_img', grid, 0 )

writer.close()