# WEB EXTREME
### A quarantine challenge for web developers

Creating a CRUD using only HTML and C++.

Only allowed libs? The ones that come with standard Ubuntu.

Also, you must parse and create HTTP requests and responses.

Your CRUD must be of a resource such as a blog post, with more than 1 field. For example: Blog post has title, body, id and poster (all simple strings).

Your resources have to be persistent. When you create a post, it should be there next time even if you shut down the server.

Bonus points: port foward your IP on your router and make it accessible by anyone.

Bonus points: implement authentication with a cryptography algorithm of your choosing.

## This is my take on the challenge

Index page:
![Index page](https://i.imgur.com/Vthe2su.png)

Show page:
![Show page](https://i.imgur.com/uuC9LCS.png)

Create page:
![Create page](https://i.imgur.com/PdYm3Gi.png)

Before compiling for the first time, run setup:
```
make setup
```

Compiling:
```
make
```

Running:
```
make run HOST=[PORT NUMBER such as 8080]
```

Dealing with the database:
```
make run FLAGS=-db
```

Using:

Go to web browser, insert 0.0.0.0:[PORT NUMBER] and there you have it!

