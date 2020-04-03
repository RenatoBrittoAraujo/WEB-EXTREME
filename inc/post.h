#ifndef POST
#define POST

#include <string>

#include "dbObject.h"

class Post : public DBObject
{
public:
  Post(std::string title, std::string body,
       std::string author) :
    title(title), body(body), author(author)
  {}
  void setTitle   (const std::string title)   
    { this->title = title; }
  void setBody    (const std::string body)    
    { this->body = body; }
  void setAuthor  (const std::string author)  
    { this->author = author; }
  std::string getTitle() { return title; }
  std::string getBody() { return body; }
  std::string getAuthor() { return author; }
private:
  std::string author;
  std::string title;
  std::string body;
};

#endif