# FhooterDemo
Just Some Code Sample + Demo(Coming Soon....)

Fhooter shooter is a multiplayer shooter template with stylized funny elements. It's the game I'm currently working on and this is made mostly using C++.

Systems already inplemented atm:
1. ServerSideRewind(SSR): 
   Server is keeping track of player's hitbox position and state for a specified time(currently 4 sec i.e. upto 400ms ping).
2. Weapons(both Hitscan and Projectile) are SSR compatible, Server Authoritative:
   Both type of weapons are implemented for using SSR functionality. That means whenever Client hit other player on client side, it will send the hit info and timestamp to server to check whether it actually hit or not by checking it against the saved hitbox data at that time.
3. Both Server and Client times are synced to facilitate SSR Logic.
4. Powerups and pickups
5. Anim Instance with all the functionalities like Hand IKs, TurnInPlace, Pitch and Yaws, etc. all mostly C++ and setups in Blueprints.
6. Custom Multiplayer Sessions Plugin to handle hosting and Joining Sessions. TODO: Server and Client Builds, and testing logics with 120fps server tick.
7. And other basic Multiplayer Shooter Functionalities.
