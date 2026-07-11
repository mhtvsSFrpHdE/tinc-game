## What are these weird dockerfile do
Docker is full of dumb ways to cache miss  
So you have to apt install download install again and again and again  
They say you should put all apt command together  
and you need another 3 tiny dependencies, great, you have to redownload all of them

If you split them to multiple RUN command,  
then after first one all other will endless download and don't get cache

Here once I installed certain amount of packages,  
build them as dedicated image, and chain them together  
Start test and develop use last image as base  
so I can do things and download no more
