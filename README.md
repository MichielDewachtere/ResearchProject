# 1000 NPC's at 60 FPS
This project is based on the GamAIPro 3 article "1000 NPCs at 60FPS" by Robert Zubek<sup>1</sup>, which is inspired by the game "Project Highrise".

## World Layout
The world, or skyscraper in this case, is build of different rooms. There are 4 types of rooms: living quarters (brown), diners (green), workplaces (red) and elevators (gray). These are random generated with a different weight to the first 3 types. Our elevators are 100x100 pixels, a perfect square, so we can make a nice grid with the elevators dimensions for each cell.

A random generated Skyscraper: 
![image](https://user-images.githubusercontent.com/78497980/213525859-8b4a18ec-d487-46e1-a73e-24e5875a8454.png)

## Agents
There are around 1000 agents in our skyscraper, each is assigned an apartment, a workstation and a day schedule. The agents wander around following their daily schedule, which can differ day to day. We use A* pathfinding to move our agents around.

## A* Pathfinding
At first glance, A* pathfinding doesn't seem very efficient for this many agents on this scale. But based on the game's design, we can interpret our elevators as nodes, and floor planes as edges, so moving inside each floor plate becomes a simple straight-line approach. For example, we have a skyscraper of 100 stories tall by 150 tiles (1 tile is 100x100 pixels) wide with 1 elevator shaft. So we can reduce the space from 15'000 grid cells (100 * 150) to 100 graph nodes and 100 edges between them.

## Conclusion
With some planning and logical thinking, a somewhat unrealistic goal, like running 1000 agents at 60 FPS, can be made extremely efficient and relatively easy.

## Source
<sup>1</sup> : http://www.gameaipro.com/GameAIPro3/GameAIPro3_Chapter34_1000_NPCs_at_60_FPS.pdf.
