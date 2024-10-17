# Simulating-Guiding-Missiles
PJ Kim / April 26th, 2024<br>
C++, SFML, Pathfinding, Computer Graphics, Pursuit Curves<br><br>

<h1>Introduction</h1>
I wanted to create a game to simulate missile pathfinding. Players move around as a tank and try to target and shoot moving fighter jets. I chose this project to learn more about pathfinding, differential equations, and C++.<br><br>

<h1>How I built it</h1>
I programmed the software in C++ using the SFML library and developed the entire program in Visual Studio.<br><br>

For the missile pathfinding, I employed several optimization techniques. To detect obstacles, I calculated the convex hull of the obstacles using a gift wrapping algorithm. The convex hull is the smallest convex polygon that contains all the points of the obstacles, which helps prevent missiles from getting stuck in concave shapes.<br><br>

For pathfinding around obstacles, I used a method inspired by ray marching. Typically used in 3D computer graphics, ray marching helped me detect the location of obstacles in the 2D space of the missile’s environment. With this information, the missile could calculate a safe path around the obstacles.<br><br>

Finally, I used differential equations to determine the missile's trajectory toward the moving target, once a clear path was found. By employing a pursuit curve, the missile follows the optimal route from its current position to the moving fighter jet.<br><br>

<h1>Conclusion</h1>
I'm happy with the results of this project. I learned a lot about pathfinding, differential equations, and C++. I also improved my problem-solving skills and gained experience in game development.<br><br>

<h1>What's Next?</h1>
There are a lot of optimizations still left for this project. One big improvement would be to implement a more efficient pathfinding algorithm such as A star or Dijkstra's algorithm. However, It was still a lot of fun developing my own pathfinding algorithm.
