# Size-Measurement : Measure the height and width of objects in image.
This repository contains code that gives you the dimensions of objects in image. The algorithm takes a image, containing a reference object whose width and height are known and other objects whose dimensions are to be determined, as input. The object should be placed such that the reference object is the leftmost in the image and all object lie on a same plane. The output of this algorithm is a image that has height and width of all objects drawn on the image provided.

#### Procedure
1. Execute the `run.py` file.
> ***Note:** This procedure is to be followed for stored images. If you want to get image on the spot then uncomment the commented part in `run.py` and press 'q' on the keyboard to capture the image*
2. It will ask for reference image width and height.
3. After entering the required information, you will get the output.
4. Press `Enter key` to exit.

#### Results
| | | |
|:-------------------------:|:-------------------------:|:-------------------------:|
| Image provided | <img src="https://github.com/AtharvaKalsekar/Computer-Vision/blob/master/SizeMeasurement/ref%20images/ref_4.jpg">  | <img src="https://github.com/AtharvaKalsekar/Computer-Vision/blob/master/SizeMeasurement/ref%20images/ref_3.jpg"> |
| Reference object | Eraser | Bottle cap |
| Reference (w,h) in centimeters | (1.7,3,5) | (2.5,2.5) |
| Predictions | <img src="https://github.com/AtharvaKalsekar/Computer-Vision/blob/master/SizeMeasurement/results/res_1.jpg"> | <img src="https://github.com/AtharvaKalsekar/Computer-Vision/blob/master/SizeMeasurement/results/res_2.jpg"> |
