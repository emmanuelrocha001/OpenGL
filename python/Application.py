import pygame, random
import numpy
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *


class Cube:

  vertices = [
    [1, -1, -1],
    [1, 1, -1],
    [-1, 1, -1],
    [-1, -1, -1],
    [1, -1, 1],
    [1, 1, 1],
    [-1, -1, 1],
    [-1, 1, 1]
    ]

  edges = [
    [0,1],
    [0,3],
    [0,4],
    [2,1],
    [2,3],
    [2,7],
    [6,3],
    [6,4],
    [6,7],
    [5,1],
    [5,4],
    [5,7]
    ]

  surfaces = [
    [0, 1, 2, 3],
    [4, 5, 7, 6],
    [2, 7, 6, 3],
    [0, 1, 5, 4],
    [0, 3, 6, 4],
    [1, 2, 7, 5],
  ]

  def __init__(self, mul=1):
    self.edges = Cube.edges
    self.vertices = list(numpy.multiply(numpy.array(Cube.vertices), mul))
    self.surfaces = Cube.surfaces



  def draw(self):
    glLineWidth(5)
    glBegin(GL_LINES)
    for edge in self.edges:
      for vertex in edge:
        print(self.vertices[vertex])
        glVertex3fv(self.vertices[vertex])
        glColor3f(1, 1, 1)
    glEnd()

  def draw_sides(self):
    glBegin(GL_QUADS)
    for surface in self.surfaces:
      for vertex in surface:
        # print(vertices[vertex])
        glVertex3fv(self.vertices[vertex])
        glColor3f(random.random(), random.random(), random.random())
    glEnd()

  def move(self, x, y, z):
    self.vertices = list(map(lambda vert: (vert[0] + x, vert[1] + y, vert[2] + z), self.vertices))

class Tesseract:

  vertices = [
    # inner cube
    [1, -1, -1],
    [1, 1, -1],
    [-1, 1, -1],
    [-1, -1, -1],

    [1, -1, 1],
    [1, 1, 1],
    [-1, 1, 1],
    [-1, -1, 1],

    # outer cube
    [2, -2, -2],
    [2, 2, -2],
    [-2, 2, -2],
    [-2, -2, -2],

    [2, -2, 2],
    [2, 2, 2],
    [-2, 2, 2],
    [-2, -2, 2],
    ]

  edges = [
    # inner edges
    [0,1],
    [0,3],
    [0,4],
    [1,2],
    [1,5],
    [2,3],
    [2,6],
    [3,7],
    [4,5],
    [4,7],
    [5,6],
    [6,7],

    # outer edges
    [8,9],
    [8,11],
    [8,12],
    [9,10],
    [9,13],
    [10,11],
    [10,14],
    [11,15],
    [12,13],
    [12,15],
    [13,14],
    [14,15],

    # connecting edges
    [1,9],
    [2,10],
    [6,14],
    [5,13],
    [0,8],
    [3,11],
    [7,15],
    [4,12],
    ]

  surfaces = [
    #back faces
    [0,1,2,3],
    [8,9,10,11],

    #front faces
    [4,5,6,7],
    [12,13,14,15],

    #left faces
    [3,2,6,7],
    [11,10,14,15],

    #right faces
    [0,1,5,4],
    [8,9,13,12],

    #bottom faces
    [4,0,3,7],
    [12,8,11,15],

    #top faces
    [1,2,6,5],
    [13,9,10,14],
  ]

  def __init__(self, mul=1):
    self.edges = Tesseract.edges
    self.vertices = list(numpy.multiply(numpy.array(Tesseract.vertices), mul))
    self.surfaces = Tesseract.surfaces



  def draw(self):
    glLineWidth(10)
    glBegin(GL_LINES)
    for edge in self.edges:
      for vertex in edge:
        print(self.vertices[vertex])
        glVertex3fv(self.vertices[vertex])
        glColor3f(random.random(), random.random(), random.random())
    glEnd()

  def draw_sides(self):
    glBegin(GL_QUADS)
    for surface in self.surfaces:
      for vertex in surface:
        # print(vertices[vertex])
        glVertex3fv(self.vertices[vertex])
        # glColor3f(0, 0, random.random())
        glColor3f(random.random(), random.random(), random.random())

    glEnd()

  def move(self, x, y, z):
    self.vertices = list(map(lambda vert: (vert[0] + x, vert[1] + y, vert[2] + z), self.vertices))

def main():
  pygame.init()
  display = (1200, 1200)
  icon = pygame.image.load( 'icon.png' )
  pygame.display.set_icon( icon )

  pygame.display.set_caption( 'Tesseract')
  pygame.display.set_mode(display, DOUBLEBUF | OPENGL)

  gluPerspective(45, (display[0]/display[1]), .1, 50)
  glTranslatef(0, 0, -40)
  # glEnable(GL_DEPTH_TEST)

  p = Cube(4)
  t = Tesseract(2)
  t2 = Tesseract(4)


  vel = 0.1
  clock = pygame.time.Clock()
  while True:
    clock.tick(60)
    for event in pygame.event.get():
      if event.type == pygame.QUIT:
        pygame.quit()
        quit()

    keys = pygame.key.get_pressed()
    if keys[pygame.K_a]:
      # glTranslatef(-vel, 0, 0)
      p.move(-vel, 0, 0)
    if keys[pygame.K_d]:
      # glTranslatef(vel, 0, 0)
      p.move(vel, 0, 0)
    if keys[pygame.K_w]:
      # glTranslatef(0, vel, 0)
      p.move(0, vel, 0)
    if keys[pygame.K_s]:
      # glTranslatef(0, -vel, 0)
      p.move(0, -vel, 0)
    if keys[pygame.K_k]:
      p.move(0, 0, vel)
    if keys[pygame.K_l]:
      p.move(0, 0, -vel)



    # glRotate(1, 1, 1, 1)
    #clears display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glRotatef(2, 1.5, 1.5, 1.5)

    # p.draw()
    t.draw()
    t2.draw()
    t.draw_sides()
    # t.draw_sides()

    # p.draw_sides()
    # p.draw_sides()
    pygame.display.flip()


if __name__ == '__main__':
  main()
