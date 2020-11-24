from models import Yolov4
model = Yolov4(weight_path='model_weights.keras', 
               class_name_path='classes.txt')
               
model.predict('test1.png')