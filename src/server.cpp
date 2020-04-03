#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void handleRequest(int);

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  clock_t initialClock = clock();

  if (argc != 2)
  {
    fprintf(stderr, "ERROR expected one argument on call. Usage: ./executable [PORT NUMBER]\n");
    exit(1);
  }

  int sockfd, newsockfd, portno, pid;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    error("ERROR on binding");
  }

  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  double timedelta = double(clock() - initialClock) / double(CLOCKS_PER_SEC);
  std::cout << std::setprecision(2);
  std::cout << "SERVER IS UP! (took " << ( timedelta < 0.01 ? "less than one second" : std::to_string(timedelta) + "second" + (timedelta >= 2.0 ? "(s)" : "") )  << ")" << std::endl;
  std::cout << "AVAILABLE AT http://localhost:" << portno << std::endl;
  std::cout << "LISTENING..." << std::endl << std::endl;

  while (true)
  {
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    pid = fork();
    if (pid == 0)
    {
      close(sockfd);
      handleRequest(newsockfd);
      exit(0);
    }
    else
    {
      close(newsockfd);
    }
  }
  close(sockfd);
  return 69420;
}

// class Request
// {
// public:
//   Request(const char* response_buffer);
// private:
//   std::string request_text;
// };

// class Response 
// {
// public:
//   Response(Request request);
//   std::string getResponse();
//   int getResponseSize();
// private:

// };

// void handleRequest2(int sock)
// {
//   const int MAX_REQUEST_SIZE = 10000;
//   char buffer[MAX_REQUEST_SIZE];
//   bzero(buffer, MAX_REQUEST_SIZE);

//   if (read(sock, buffer, MAX_REQUEST_SIZE) < 0)
//   {
//     error("ERROR at recieving request");
//   }

//   std::cout << "==== REQUEST:" << std::endl << buffer << std::endl;

//   Request req(buffer);
//   Response res(req);

//   std::cout << "==== RESPONSE:" << std::endl << res.getResponse() << std::endl;

//   if (write(sock, res.getResponse().c_str(), res.getResponseSize()) < 0)
//   {
//     error("ERROR at sending response");
//   }
// }

void handleRequest(int sock)
{
  const int MAX_REQUEST_SIZE = 10000;
  char buffer[MAX_REQUEST_SIZE];
  bzero(buffer, MAX_REQUEST_SIZE);

  if (read(sock, buffer, MAX_REQUEST_SIZE) < 0)
  {
    error("ERROR at recieving request");
  }

  std::cout<<"==== REQUEST:" << std::endl << buffer << std::endl;

  std::string s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

  std::ifstream file("assets/index.html", std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> bufferr(size);

  if (file.read(bufferr.data(), size))
  {
    std::string vs;
    for (auto v : bufferr)
      if (v == '\n')
        vs += "\r\n";
      else
        vs += v;
    vs += "\r\n";
    s += vs;
    std::cout << "==== RESPONSE:" << std::endl << s << std::endl;
    if (write(sock, s.c_str(), (int)s.size()) < 0)
    {
      error("ERROR at sending response");
    }
    std::cout << "RESPONSE COMPLETE, LISTENING..." << std::endl << std::endl;
  }
}
