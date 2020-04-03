#ifndef DB_OBJECT
#define DB_OBJECT

class DBObject
{
public:
  void setID (int id) { this->id = id; }
  int getID () { return id; }
private:
  int id;
};

#endif