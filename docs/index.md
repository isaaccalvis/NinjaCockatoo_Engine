# RedCircle Engine
3D Video Game Engine made with C++ & OpenGL

Welcome to the repository of RedCircle Engine !

RedCircle is a Video Game Engine developed by 2 students from the Video Game Development & Design degree, at CITM (UPC).

![RedCircleEngine](EngineShot.png)

## About our team

RedCircle engine is developed by:
  - Isaac Calvís [GitHub](https://github.com/isaaccalvis)
  - Aitor Velez [GitHub](https://github.com/AitorVelez)



## Main Core Sub-Systems

 - Module Scene : Manages all the scene and all ingame components attatched. We can save and load scenes in binary format.
 
 - Hirearchy : Manages all game objects and its properties in game.
 
 - Importer System : We can import models and its textures and convert it into our own file format.
 
 
 - Ingame Timing : We have a play button to view the game from the main camera.Time mainly affects to our phyisics update.
 
 
## Phyisics engine
We implemented colliders, rigidbodies and phyisics constrains ,these allows use to have the core parts of a phisics engine. In our engine you are able to test if two objects collide, apply gravity to them, and also apply collider component.
In this gif it' s possible to see two objetcs that have been previously applyied a rigidbody and a collider.

![Collider](ColliderAndRigidBody.gif)

You have a physics panel where:
  - Can Activate/ Deactivate physics
  - Draw Physics debug
  - Modify gravity force
  - Clear all rigidbodies forces

### Physic Objects properties
Colliders:
  - Collider size (for each axis)
  - Collider center point
  - Type of shape (Cube & Sphere)
  - Trigger option
  - And also will show with wich objects is colliding
 
RigidBodies:
  - Type of shape (Cube or Sphere)
  - You can modifi RigidBody size (for each axis)
  - Mass
  - Use Gravity
  - Clear forces button
  - And also will show with wich objects is colliding

Constraints:
  - Choose constraint type (P2P, Hinge, Slider)
  - Connect Game Object button (click button and then the GO to connect, it will show the name of connected GO)
  - BodyA point
  - BodyB point
  - AxisA point
  - AxisB point

## Downloads & Release

[Source Code](https://github.com/isaaccalvis/RedCircle_Engine)
[Releases](https://github.com/isaaccalvis/RedCircle_Engine/releases)

### Use Scene
You will find at folder Library/Scenes the file PhysicsScene.sscene, you can load it dragging on the engine or using the load scenes panel. Theres a Sphere with a camera that have a rigidBody, you can move & shot balls. Also you'll find 2 cubes, one with a RigidBody & another one with a RigidBody & Constraint, choose what constraint you want, the points, and connect them using the button at Constraint properties.

## Controls

 - WASD: Navigation forward, backward and sides.
 - ER: Navigation up and down.
 - F: Center the camera in the selected object.
 - LeftAlt: Orbital camera around selected game object
 - Mouse Wheel: Zoom in, zoom out
 - Hold Shift: Camera moves x2 fast
 - Del: To delete selected Game Object
 - RigthMouse Button to move camera

Guizmos

 - Q: Change between Local & Global Mode
 - W: Translate
 - E: Rotate
 - R: Scale
 
 InGame
 - WASD: Navigation forward, backward and sides.
 - ER: Navigation up and down.
 - RigthMouse Button to move camera
 - Space to shot a ball

## Panels

 - Esc: Quit
 - F1: Shows configuration panel
 - F2: Shows  object propierties panel
 - F3: Shows console panel
 - F4: Shows hirearchy panel
 - F5: Shows QuadTree panel
 - F6: Shows PlayGame panel
 - F7: Shows Asset panel
 - F8: Shows Physics panel
 - F9: Shows the about panel
 
## Features

 - Drag&Drop 3D models & textures from your pc to the GameEngine, it will be added to the project at folder resources
 - Create & Delete GameObjects at Hierachy Panel
 - Create Primitives &/or add components to game objects (at top bar, gameobjects & components)
 - At configuration window can manage render & window options, also draw in wireframe mode or draw imported meshes normals, with a lot more features !
 - At properties panel you can modify selected GameObject transform, also see how many triangles have, or the texture associated at Material componentç
 - Create QuadTree with static GameObjects
 - Drag & Drop function at Hierachy Panel
 - Save/ Load Scenes

## Video

<iframe width="1903" height="768" src="https://www.youtube.com/embed/WZSejnIXudA" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Used Tools & libraries

Tools
  - Visual Studio 2017

Libraries
 - SDL
 - OpenGL
 - Glew
 - Assimp
 - DevIL
 - Imgui
 - MathGeoLib
 - Parson
 - MMGR
 - PCG Random
 - Par Shapes
 - PhysFS
 - ImGuizmo
 - BulletPhysics

## License

Copyright <2019> <RedCircle_Engine>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
