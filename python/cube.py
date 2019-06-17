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

def main():
  pygame.init()
  display = (1200, 1200)
  icon = pygame.image.load( 'icon.png' )
  pygame.display.set_icon( icon )

  pygame.display.set_caption( 'cube')
  pygame.display.set_mode(display, DOUBLEBUF | OPENGL)

  gluPerspective(45, (display[0]/display[1]), .1, 50)
  glTranslatef(0, 0, -20)
  glEnable(GL_DEPTH_TEST)

  p = Cube(2)
  p1 = Cube(1)
  p2 = Cube(3)
  p3 = Cube(4)

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
    glRotatef(1.5, 1.5, 1.5, 1.5)

    p.draw()
    p1.draw()
    p2.draw()
    p3.draw()
    p.draw_sides()
    # p.draw_sides()
    pygame.display.flip()


if __name__ == '__main__':
  main()
