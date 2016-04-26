# HumanRouting

This is a project presented at Copenhackathon 2016.
A human router device was implemented using and arduino microcontroller and a set of routing algorithms.

# About the algorithm

The algorithm is given the current position of the user, as well as all the points the user would like to pass from. The algorithm will then attempt to find the shortest path possible passing through all points. (Note: the problem is not the same as the traveling salesman problem, a user does not care passing the same point multiple times as long as by the end it is the shortest possible path)

For this the Floyd-Warshall algorithm was used to calculate all pairs shortest paths. Further exploration is done using a non-standard algorithm developed during the hackathon day. It is not complete due to time constraints however covers most cases. 

# Future work

1. Fix bugs in the path calculation, probably using dynamic programming.
2. Organize spaghetti code. This is what happens when everything need to be done within 2 days... and the working area closing at midnight...
3. Move from static routing to dynamic routing
