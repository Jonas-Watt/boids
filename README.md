# boids
https://en.wikipedia.org/wiki/Boids

also https://eater.net/boids and https://www.youtube.com/watch?v=4LWmRuB-uNU

![Image 1](boids with predators/pictures/Bildschirmfoto 2021-10-05 um 16.31.15.png?raw=true "Title")

## Rules:
Flock/Swarm:
  - avoid collisions between boids
  - match velocity of surrounding boids
  - fly towards the center of mass of surrounding boids
  - avoid the predator
 
 Predator:
  - fly towards the center of mass of surrounding boids (more vision than the Flock)
