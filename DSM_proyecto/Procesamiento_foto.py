import matplotlib.pyplot as plt
import numpy as np
import cv2 as cv
from pide_fotos import *
from matplotlib.colors import hsv_to_rgb

# fecha = pide_foto()

# img_original = cv.imread(fecha)
# rgb_img = cv.cvtColor(img, cv.COLOR_BGR2RGB)

img_original = cv.imread('2022-12-02_203537.jpg')
img_p = cv.cvtColor(img_original, cv.COLOR_BGR2RGB)

img = cv.cvtColor(img_original, cv.COLOR_BGR2GRAY)
img = cv.medianBlur(img,11)

img_p1, img = cv.threshold(img,127,255,cv.THRESH_BINARY_INV)

kernel = np.ones((9,9), np.uint8) 
  
img = cv.dilate(img, kernel, iterations=2) 
img = cv.erode(img, kernel, iterations=2) 

# 2. Mascara
# 1280x1024
mascara = np.zeros((img.shape),dtype = np.uint8)
mascara[100:800,400:1200] = 255
img = cv.bitwise_and(img,mascara)
# plt.imshow(img,cmap='gray')
plt.show()

area = 0
for i in range(img.shape[0]):
    for j in range(img.shape[1]):
        area += img[i,j]
area = area/255

print("Área total: ", area )

plt.figure()
plt.subplot(121), plt.imshow(img_p,cmap='gray'), plt.title("Imagen original")
plt.subplot(122), plt.imshow(img,cmap='gray'), plt.title("filtrada")
plt.show()

'''
# cv.imwrite('savedImage.jpg', rgb_img)
# plt.imshow(rgb_img)
# plt.show()

hsv_img = cv.cvtColor(rgb_img, cv.COLOR_RGB2HSV)

S_claro = 255/100*28
V_claro = 255/100*60

S_oscuro = 255/100*66
V_oscuro = 255/100*58

comida_oscuro = (10, S_oscuro, V_oscuro)
comida_claro = (30, S_claro, V_claro)

lo_square = np.full((10, 10, 3), comida_claro, dtype=np.uint8) / 255.0
do_square = np.full((10, 10, 3), comida_oscuro, dtype=np.uint8) / 255.0
plt.subplot(1, 2, 1)
plt.imshow(hsv_to_rgb(lo_square))
plt.subplot(1, 2, 2)
plt.imshow(hsv_to_rgb(do_square))
plt.show()

mascara = cv.inRange(hsv_img, comida_claro, comida_oscuro)

plt.imshow(mascara,cmap='gray')
plt.show()
'''