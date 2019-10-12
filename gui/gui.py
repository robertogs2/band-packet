from tkinter import *
from PIL import ImageTk,Image
import threading
from threading import Thread
import os
import time

class App:

  def __init__(self):
    self.directory = os.path.dirname(os.path.realpath(__file__))
    print(self.directory)
    self.running = True
    self.band_start = 190
    self.band_end = 1090

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

    self.gears_image_0_label = Label(self.root_canvas, image=self.gears_image)
    self.gears_image_0_label.place(x=190, y=150)

    self.gears_image_1_label = Label(self.root_canvas, image=self.gears_image)
    self.gears_image_1_label.place(x=190, y=410)

    self.gears_image_2_label = Label(self.root_canvas, image=self.gears_image)
    self.gears_image_2_label.place(x=190, y=670)


    self.box_image_0_label = Label(self.root_canvas, image=self.box_radioactive)
    self.box_image_0_label.place(x=190, y=90)

    self.box_image_1_label = Label(self.root_canvas, image=self.box_normal)
    self.box_image_1_label.place(x=190, y=350)

    self.box_image_2_label = Label(self.root_canvas, image=self.box_urgent)
    self.box_image_2_label.place(x=190, y=610)

    self.box_id_0_label = Label(self.root_canvas, text="Current package id:")
    self.box_id_0_label.place(x=530, y=30)

    self.box_id_1_label = Label(self.root_canvas, text="Current package id:")
    self.box_id_1_label.place(x=530, y=280)

    self.box_id_2_label = Label(self.root_canvas, text="Current package id:")
    self.box_id_2_label.place(x=530, y=560)


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
      pkg_id, pkg_progress, pkg_type, pkg_side= self.get_box_strings("0")

      if(pkg_id != old_pkg_id and pkg_id != "-1"):
        self.box_id_0_label.config(text="Current package id: " + pkg_id)
        old_pkg_id = pkg_id

      if((pkg_progress != old_pkg_progress and pkg_progress != "") or (pkg_side != old_pkg_side and pkg_side != "")):
        if(pkg_side == 0):
          pos = (pkg_progress * 900 // 100)
          if(pkg_progress > 100):
            pos = 900

        elif(pkg_side == 1):
          pos = 840 - (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = -60

        old_pkg_side = pkg_side
        old_pkg_progress = pkg_progress
        self.box_image_0_label.place(x=pos + self.band_start)

      if(pkg_type != old_pkg_type and pkg_type != ""):
        old_pkg_type = pkg_type
        if(pkg_type == 3):
          self.box_image_0_label.config(image=self.box_normal)
        elif(pkg_type == 2):
          self.box_image_0_label.config(image=self.box_urgent)
        elif(pkg_type == 1):
          self.box_image_0_label.config(image=self.box_radioactive)
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
      pkg_id, pkg_progress, pkg_type, pkg_side = self.get_box_strings("1")

      if (pkg_id != old_pkg_id and pkg_id != "-1"):
        self.box_id_1_label.config(text="Current package id: " + pkg_id)
        old_pkg_id = pkg_id

      if ((pkg_progress != old_pkg_progress and pkg_progress != "") or (pkg_side != old_pkg_side and pkg_side != "")):
        if (pkg_side == 0):
          pos = (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = 900

        elif (pkg_side == 1):
          pos = 840 - (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = -60

        old_pkg_side = pkg_side
        old_pkg_progress = pkg_progress
        self.box_image_1_label.place(x=pos + self.band_start)

      if (pkg_type != old_pkg_type and pkg_type != ""):
        old_pkg_type = pkg_type
        if (pkg_type == 3):
          self.box_image_1_label.config(image=self.box_normal)
        elif (pkg_type == 2):
          self.box_image_1_label.config(image=self.box_urgent)
        elif (pkg_type == 1):
          self.box_image_1_label.config(image=self.box_radioactive)

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
      pkg_id, pkg_progress, pkg_type, pkg_side = self.get_box_strings("2")

      if (pkg_id != old_pkg_id and pkg_id != "-1"):
        self.box_id_2_label.config(text="Current package id: " + pkg_id)
        old_pkg_id = pkg_id

      if ((pkg_progress != old_pkg_progress and pkg_progress != "") or (pkg_side != old_pkg_side and pkg_side != "")):
        if (pkg_side == 0):
          pos = (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = 900

        elif (pkg_side == 1):
          pos = 840 - (pkg_progress * 900 // 100)
          if (pkg_progress > 100):
            pos = -60

        old_pkg_side = pkg_side
        old_pkg_progress = pkg_progress
        self.box_image_2_label.place(x=pos + self.band_start)

      if (pkg_type != old_pkg_type and pkg_type != ""):
        old_pkg_type = pkg_type
        if (pkg_type == 3):
          self.box_image_2_label.config(image=self.box_normal)
        elif (pkg_type == 2):
          self.box_image_2_label.config(image=self.box_urgent)
        elif (pkg_type == 1):
          self.box_image_2_label.config(image=self.box_radioactive)

      else:
        #do nothing if nothing has changed
        pass

  def get_box_strings(self, id_band):
    f = open(self.directory+"/data/band_"+id_band+".txt")
    pkg_id = "-1"
    pkg_progress = ""
    pkg_type = ""
    pkg_side = "" #left -> right, 1 #left <- right


    lines = f.readlines()
    if(lines != []):
      pkg_id = lines[0]
      pkg_progress = int(lines[1])
      pkg_type = int(lines[2])
      pkg_side = int(lines[3])
    return pkg_id, pkg_progress, pkg_type, pkg_side

app = App()