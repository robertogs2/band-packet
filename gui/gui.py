from tkinter import *
from PIL import ImageTk,Image
import threading
from threading import Thread, RLock
import os
import time
import serial

class App:

  def __init__(self):
    self.directory = os.path.dirname(os.path.realpath(__file__))
    print(self.directory)
    self.running = True
    self.band_start = 190
    self.band_end = 1090
    algoritms = self.get_const_strings()


    self.serial_port = "/dev/ttyACM0"
    self.serial_baudrate = 115200
    self.serial_timeout = 0.2
    self.ardu = None
    self.mutex= RLock()

    try:
      self.ardu=serial.Serial(self.serial_port, self.serial_baudrate, timeout=self.serial_timeout)
    except Exception as e:
      print(e)
    self.root = Tk()
    self.root.title("Band Packet Simulation")
    self.root.geometry("1280x920")
    self.root.resizable(width=NO, height=NO)
    self.root.protocol('WM_DELETE_WINDOW', self.exit)

    self.root_canvas = Canvas(self.root, width=1280, height=920)
    self.root_canvas.grid(row=0, column=0)

    self.gears_image = self.load_img(self.directory + "/images/gears_0.png")

    self.box_radioactive = self.load_img(self.directory + "/images/box_radioactive.png")
    self.box_urgent = self.load_img(self.directory + "/images/box_urgent.png")
    self.box_normal = self.load_img(self.directory + "/images/box_normal.png")
    self.sign_left = self.load_img(self.directory + "/images/sign_left.png")
    self.sign_right = self.load_img(self.directory + "/images/sign_right.png")

    self.gears_image_0_label = Label(self.root_canvas, image=self.gears_image)
    self.gears_image_0_label.place(x=190, y=150)

    self.algorithms_0_label = Label(self.root_canvas, text = "Scheduler: " + algoritms[0] +"Control: " + algoritms[1], fg="yellow", justify=LEFT)
    self.algorithms_0_label.place(x=190, y=30)

    self.gears_image_1_label = Label(self.root_canvas, image=self.gears_image)
    self.gears_image_1_label.place(x=190, y=410)

    self.algorithms_1_label = Label(self.root_canvas, text="Scheduler: " + algoritms[2] + "Control: " + algoritms[3], fg="yellow", justify=LEFT)
    self.algorithms_1_label.place(x=190, y=280)

    self.gears_image_2_label = Label(self.root_canvas, image=self.gears_image)
    self.gears_image_2_label.place(x=190, y=670)

    self.algorithms_2_label = Label(self.root_canvas, text="Scheduler: " + algoritms[4] + "Control: " + algoritms[5], fg="yellow", justify=LEFT)
    self.algorithms_2_label.place(x=190, y=560)

    self.sign_image_0_label = Label(self.root_canvas, image=self.sign_right)
    self.sign_image_0_label.place(x=990, y=30)

    self.sign_image_1_label = Label(self.root_canvas, image=self.sign_right)
    self.sign_image_1_label.place(x=990, y=290)

    self.sign_image_2_label = Label(self.root_canvas, image=self.sign_right)
    self.sign_image_2_label.place(x=990, y=550)

    self.box_image_0_label = Label(self.root_canvas, image=self.box_radioactive)
    self.box_image_0_label.place(x=190, y=90)

    self.box_image_1_label = Label(self.root_canvas, image=self.box_normal)
    self.box_image_1_label.place(x=190, y=350)

    self.box_image_2_label = Label(self.root_canvas, image=self.box_urgent)
    self.box_image_2_label.place(x=190, y=610)

    self.box_id_0_label = Label(self.root_canvas, text="Current package id:")
    self.box_id_0_label.place(x=530, y=30)

    self.band_0_left_queue_label = Label(self.root_canvas, text="Id:0 R\nId:0 R\nId:0 R\nId:0 R\nId:0 R")
    self.band_0_left_queue_label.place(x=50, y=30)

    self.band_0_right_queue_label = Label(self.root_canvas, text="Id:0 R\nId:0 R\nId:0 R\nId:0 R\nId:0 R")
    self.band_0_right_queue_label.place(x=1180, y=30)


    self.box_id_1_label = Label(self.root_canvas, text="Current package id:")
    self.box_id_1_label.place(x=530, y=280)

    self.band_1_left_queue_label = Label(self.root_canvas, text="Id:0 R\nId:0 R\nId:0 R\nId:0 R\nId:0 R")
    self.band_1_left_queue_label.place(x=50, y=280)

    self.band_1_right_queue_label = Label(self.root_canvas, text="Id:0 R\nId:0 R\nId:0 R\nId:0 R\nId:0 R")
    self.band_1_right_queue_label.place(x=1180, y=280)

    self.box_id_2_label = Label(self.root_canvas, text="Current package id:")
    self.box_id_2_label.place(x=530, y=560)

    self.band_2_left_queue_label = Label(self.root_canvas, text="Id:0 R\nId:0 R\nId:0 R\nId:0 R\nId:0 R")
    self.band_2_left_queue_label.place(x=50, y=560)

    self.band_2_right_queue_label = Label(self.root_canvas, text="Id:0 R\nId:0 R\nId:0 R\nId:0 R\nId:0 R")
    self.band_2_right_queue_label.place(x=1180, y=560)


    self.thread_gears_0 =  Thread(target=self.update_gears, args=())
    self.thread_gears_0.start()

    self.thread_box_0 = Thread(target=self.update_box_0, args=())
    self.thread_box_0.start()

    self.thread_box_1 = Thread(target=self.update_box_1, args=())
    self.thread_box_1.start()

    self.thread_box_2 = Thread(target=self.update_box_2, args=())
    self.thread_box_2.start()

    self.root.mainloop()

  def exit(self):
    self.running = False
    raise SystemExit("Successfully closed")

  def load_img(self, path):
    img = ImageTk.PhotoImage(Image.open(path))
    return img

  def update_gears(self):
    img_num = 0
    while(self.running):
      if(img_num == 0):
        img_num = 1
      else:
        img_num = 0
      self.gears_image = self.load_img(self.directory + "/images/gears_"+str(img_num)+".png")
      self.gears_image_0_label.config(image=self.gears_image)
      self.gears_image_1_label.config(image=self.gears_image)
      self.gears_image_2_label.config(image=self.gears_image)
      time.sleep(0.5)
    return

  def update_box_0(self):
    #pos = (pos_read * 900) / 100
    old_pkg_id = ""
    old_pkg_progress = 0
    old_pkg_type = 3
    old_pkg_side = 0

    while(self.running):
      pkg_id, pkg_progress, pkg_type, pkg_side, list_left, list_right = self.get_box_strings("0")

      if(pkg_id != old_pkg_id and pkg_id != "-1"):
        self.box_id_0_label.config(text="Current package id: " + pkg_id)
        old_pkg_id = pkg_id

      if((pkg_progress != old_pkg_progress and pkg_progress != "") or (pkg_side != old_pkg_side and pkg_side != "")):
        if(pkg_side == 0):
          pos = (pkg_progress * 900 // 100)
          if(pkg_progress > 100):
            pos = 900
          self.sign_image_0_label.config(image=self.sign_right)

        elif(pkg_side == 1):
          pos = 840 - (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = -60
          self.sign_image_0_label.config(image=self.sign_left)
        self.write_hardware(0, "sign", 1-pkg_side)
        self.write_hardware(0, "packet", 100-pkg_progress if pkg_side else pkg_progress)
        old_pkg_side = pkg_side
        old_pkg_progress = pkg_progress
        self.box_image_0_label.place(x=pos + self.band_start)
        self.band_0_left_queue_label.config(text="Next in line:\n" + list_left)
        self.band_0_right_queue_label.config(text="Next in line:\n" +list_right)

      if(pkg_type != old_pkg_type and pkg_type != ""):
        old_pkg_type = pkg_type
        if(pkg_type == 2):
          self.box_image_0_label.config(image=self.box_normal)
        elif(pkg_type == 1):
          self.box_image_0_label.config(image=self.box_urgent)
        elif(pkg_type == 0):
          self.box_image_0_label.config(image=self.box_radioactive)
        self.write_hardware(0, "rgb", pkg_type)
      else:
        #do nothing if nothing has changed
        pass

  def update_box_1(self):
    #pos = (pos_read * 900) / 100
    old_pkg_id = ""
    old_pkg_progress = 0
    old_pkg_type = 3
    old_pkg_side = 0

    while(self.running):
      pkg_id, pkg_progress, pkg_type, pkg_side, list_left, list_right = self.get_box_strings("1")

      if (pkg_id != old_pkg_id and pkg_id != "-1"):
        self.box_id_1_label.config(text="Current package id: " + pkg_id)
        old_pkg_id = pkg_id

      if ((pkg_progress != old_pkg_progress and pkg_progress != "") or (pkg_side != old_pkg_side and pkg_side != "")):
        if (pkg_side == 0):
          pos = (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = 900
          self.sign_image_1_label.config(image=self.sign_right)

        elif (pkg_side == 1):
          pos = 840 - (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = -60
          self.sign_image_1_label.config(image=self.sign_left)
        self.write_hardware(1, "sign", 1-pkg_side)
        self.write_hardware(1, "packet", 100-pkg_progress if pkg_side else pkg_progress)
        old_pkg_side = pkg_side
        old_pkg_progress = pkg_progress
        self.box_image_1_label.place(x=pos + self.band_start)
        self.band_1_left_queue_label.config(text="Next in line:\n" +list_left)
        self.band_1_right_queue_label.config(text="Next in line:\n" +list_right)

      if (pkg_type != old_pkg_type and pkg_type != ""):
        old_pkg_type = pkg_type
        if (pkg_type == 2):
          self.box_image_1_label.config(image=self.box_normal)
        elif (pkg_type == 1):
          self.box_image_1_label.config(image=self.box_urgent)
        elif (pkg_type == 0):
          self.box_image_1_label.config(image=self.box_radioactive)
        self.write_hardware(1, "rgb", pkg_type)
      else:
        #do nothing if nothing has changed
        pass

  def update_box_2(self):
    #pos = (pos_read * 900) / 100
    old_pkg_id = ""
    old_pkg_progress = 0
    old_pkg_type = 3
    old_pkg_side = 0

    while(self.running):
      pkg_id, pkg_progress, pkg_type, pkg_side, list_left, list_right = self.get_box_strings("2")

      if (pkg_id != old_pkg_id and pkg_id != "-1"):
        self.box_id_2_label.config(text="Current package id: " + pkg_id)
        old_pkg_id = pkg_id

      if ((pkg_progress != old_pkg_progress and pkg_progress != "") or (pkg_side != old_pkg_side and pkg_side != "")):
        if (pkg_side == 0):
          pos = (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = 900

          self.sign_image_2_label.config(image=self.sign_right)

        elif (pkg_side == 1):
          pos = 840 - (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = -60
          self.sign_image_2_label.config(image=self.sign_left)
        self.write_hardware(2, "sign", 1-pkg_side)
        self.write_hardware(2, "packet", 100-pkg_progress if pkg_side else pkg_progress)
        old_pkg_side = pkg_side
        old_pkg_progress = pkg_progress
        self.box_image_2_label.place(x=pos + self.band_start)
        self.band_2_left_queue_label.config(text="Next in line:\n" +list_left)
        self.band_2_right_queue_label.config(text="Next in line:\n" +list_right)

      if (pkg_type != old_pkg_type and pkg_type != ""):
        old_pkg_type = pkg_type
        if (pkg_type == 2):
          self.box_image_2_label.config(image=self.box_normal)
        elif (pkg_type == 1):
          self.box_image_2_label.config(image=self.box_urgent)
        elif (pkg_type == 0):
          self.box_image_2_label.config(image=self.box_radioactive)
        self.write_hardware(2, "rgb", pkg_type)
      else:
        #do nothing if nothing has changed
        pass

  def get_box_strings(self, id_band):
    f = open(self.directory+"/data/band_"+id_band+".txt")
    pkg_id = "-1"
    pkg_progress = ""
    pkg_type = ""
    pkg_side = "" #left -> right, 1 #left <- right
    list_right = ""
    list_left = ""

    lines = f.readlines()
    if(lines != []):
      if(len(lines)>= 6):
        pkg_id = lines[0]
        pkg_progress = int(lines[1])
        pkg_type = int(lines[2])
        pkg_side = int(lines[3])
        list_left = lines[4].replace(",","\n").replace(":0", ":R").replace(":1",":U").replace(":2",":N")
        list_right = lines[5].replace(",","\n").replace(":0", ":R").replace(":1",":U").replace(":2",":N")

    return pkg_id, pkg_progress, pkg_type, pkg_side, list_left, list_right

  def get_const_strings(self):
    f = open(self.directory+"/data/algorithms.txt")
    lines = f.readlines()
    return lines
  def write_hardware(self, band, command, state):
    if self.ardu is not None:
      self.mutex.acquire()
      line = command + ":" + str(band) + ":" + str(state)
      #print("Sennding: " + line)
      self.ardu.write(line.encode())
      #print("Sent")
      inp = self.ardu.readline().decode("utf-8") 
      #print(inp)
      if(inp.startswith("pins", 0)):
        splitted = inp.split("-")
        button0 = str(splitted[1])
        #print("Button0: " +button0)
        button1 = str(splitted[2])
        #print("Button1: " +button1)
        button2 = str(splitted[3])
        #print("Button2: " +button2)
        button_file = open(f'{self.directory}/data/buttons.txt', "w+")
        button_file.write(f'{button0}{button1}{button2}')
        button_file.close()
      self.mutex.release()

app = App()