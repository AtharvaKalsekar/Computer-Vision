import cv2
import numpy as np

def get_contours(frame):
    image_copy = frame.copy()
    gray = cv2.cvtColor(image_copy,cv2.COLOR_BGR2GRAY)
    g_blur = cv2.GaussianBlur(gray,(7,7),sigmaX=1)
    edged = cv2.Canny(g_blur,20,160)
    kernel = np.ones((5,5), np.uint8)
    dilate = cv2.dilate(edged,kernel,iterations=1)
    erode = cv2.erode(dilate,kernel,iterations=1)
    cv2.imshow("pro",erode)
    #cv2.waitKey(0)
    _, contours, _ = cv2.findContours(erode, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    refined_conts = [c for c in contours if(cv2.contourArea(c)>500)]
    #print(refined_conts)
    #for i,c in enumerate(refined_conts):
    #    print(i,cv2.contourArea(c))
    sorted_conts = sorted(refined_conts,key=lambda c:cv2.boundingRect(c)[0])
    #for i,c in enumerate(sorted_conts):
    #    print(i,cv2.contourArea(c),cv2.boundingRect(c)[0])
    return refined_conts

def get_rect_image(frame,contours,w_PPM,h_PPM):
    image_copy = frame.copy()
    for c in contours:
        rect = cv2.boundingRect(c)
        x,y,w,h = rect
        print("EEE : ",w,h)
        cv2.rectangle(image_copy,(x,y),(x+w,y+h),(0,255,0),2)
        text = "W = "+"{0:.2f}".format(w/w_PPM)+", H = "+"{0:.2f}".format(h/h_PPM)
        cv2.putText(image_copy, text, (x, y+h+10), cv2.FONT_HERSHEY_SIMPLEX, 0.3, (0, 0, 255), lineType=cv2.LINE_AA)
    return image_copy

def get_PPM(contour,w_ref,h_ref):
    x,y,w,h = cv2.boundingRect(contour)
    #print(w,h)
    return w/w_ref,h/h_ref

'''cap = cv2.VideoCapture(0)
if (cap.isOpened()== False): 
    print("Error opening video stream or file")
while(cap.isOpened()):
    
    ret, frame = cap.read()
    
    if ret == True:
        cv2.imshow('Frame',frame)
    
    if cv2.waitKey(25) == ord('q'):
        break
    else:
        continue

cap.release()
cv2.destroyAllWindows()
'''
w_ref = float(input("Enter width of reference object : "))
h_ref = float(input("Enter height of reference object : "))
frame = cv2.imread("reference3.jpg")
frame = cv2.resize(frame,(640,360))
cv2.imshow("org",frame)
contours = get_contours(frame)
w_PPM, h_PPM = get_PPM(contours[0],w_ref,h_ref)
marked_image = get_rect_image(frame,contours,w_PPM,h_PPM)
cv2.imshow("marked",marked_image)
cv2.imwrite("res_2.jpg",marked_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
