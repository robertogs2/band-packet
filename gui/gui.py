from tkinter import *
from PIL import ImageTk,Image
import threading
from threading import Thread
import os

class App:

  def __init__(self):
    self.directory = os.path.dirname(os.path.realpath(__file__))
    print(self.directory)
    self.running = True

    self.root = Tk()
    self.root.title("Band Packet Simulation")
    self.root.geometry("1280x920")
    self.root.resizable(width=NO, height=NO)
    self.root.protocol('WM_DELETE_WINDOW', self.exit)

    self.root_canvas = Canvas(self.root, width=1280, height=920)
    self.root_canvas.grid(row=0, column=0)

    # self.background_image = self.load_img("images/background.jpg")
    # self.background_image_label = Label(self.root_canvas, image=self.background_image, width=1280, height=920)
    # self.background_image_label.grid(row=0,column=0)

    self.gears_image_0 = self.load_img("images/gears_0.png")
    self.gears_image_0_label = Label(self.root_canvas, image=self.gears_image_0)
    self.gears_image_0_label.grid(row=0,column=0)


    self.root.mainloop()

  def exit(self):
    self.running = False
    raise SystemExit("Successfully closed")

  def load_img(self, path):
    img = ImageTk.PhotoImage(Image.open(path))
    return img

app = App()