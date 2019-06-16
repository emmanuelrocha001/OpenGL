import pygame, random
import numpy
from pygame.locals import *

from OpenGL.GL import *
from OpenGL.GLU import *




class Pyramid:

  vertices = (
    (1,-1,-1),
    (1, 1, -1),
    (-1, 1, -1),
    (-1,-1,-1),
    (0, 0, 1),
  )

  edges = (
    (0,1),
    (0,3),
    (0,4),
    (1,4),
    (1,2),
    (2,4),
    (2,3),
    (3,4),
  )
  def __init__(self, mul=1):
    self.edges = Pyramid.edges
    self.vertices = Pyramid.vertices


  def draw(self):
    glLineWidth(5);
    glBegin(GL_LINES)
    for edge in self.edges:
      for vertex in edge:
        # print(vertices[vertex])
        glVertex3fv(self.vertices[vertex])
        glColor3f(random.random(), random.random(), random.random())
    glEnd()

  def move(self, x, y, z):
    self.vertices = list(map(lambda vert: (vert[0] + x, vert[1] + y, vert[2] + z), self.vertices))

def main():
  pygame.init()
  display = (1200, 1200)
  pygame.display.set_mode(display, DOUBLEBUF | OPENGL)

  gluPerspective(45, (display[0]/display[1]), .1, 50)
  glTranslatef(0, 0, -5)

  p = Pyramid()

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
      glTranslatef(-vel, 0, 0)
    if keys[pygame.K_d]:
      glTranslatef(vel, 0, 0)
    if keys[pygame.K_w]:
      glTranslatef(0, vel, 0)
    if keys[pygame.K_s]:
      glTranslatef(0, -vel, 0)
    if keys[pygame.K_g]:
      glTranslatef(0, 0, vel)
    if keys[pygame.K_t]:
      glTranslatef(0, 0, -vel)



    # glRotate(1, 1, 1, 1)
    #clears display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    p.draw()
    pygame.display.flip()


if __name__ == '__main__':
  main()
