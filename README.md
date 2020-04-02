# WEB EXTREME
### A quarentine challange for web developers

Creating a CRUD using only HTML and C++.
Only allowed libs? The ones that come with standard Ubuntu.
Also, you must parse and create HTTP requests and responses.
Your CRUD must be of a resource such as a blog post, with more than 1 field. For example: Blog post has title, body, id and poster (all simple strings).
Your resources has to be persistent. When you create a post, it should be there next time even if you shut down the server.
Bonus points: port foward your IP on your router and make it accessible by anyone.

## This is my take on the challenge

Compiling:
```
g++ -std=c++11 server.cpp
```

Running:
```
./a.out [PORT NUMBER such as 8002]
```

Using:
Go to web browser, insert 0.0.0.0:[PORT NUMBER] and there you have it!