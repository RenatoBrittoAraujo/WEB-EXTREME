#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "request.h"
#include "response.h"
#include "database.h"

const int MAX_REQUEST_SIZE = 10000;
const int MAX_OUTPUT_SIZE = 1000;

void handleRequest(int);
void runMigrations();

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  runMigrations();

  clock_t initialClock = clock();

  if (argc != 2)
  {
    fprintf(stderr, "ERROR expected one argument on call. Usage: ./executable [PORT NUMBER] or ./executable -db\n");
    exit(1);
  }

  if (std::string(argv[1]) == "-db")
  {
    Database::DBMode();
    return 0;
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
  std::cout << "SERVER IS UP! (took " << ( timedelta < 0.01 ? 
    "less than one second" : std::to_string(timedelta) + "second" + 
    (timedelta >= 2.0 ? "(s)" : "") )  << ")" << std::endl;
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
  return 69 + 420;
}

void handleRequest(int sock)
{
  char buffer[MAX_REQUEST_SIZE];
  bzero(buffer, MAX_REQUEST_SIZE);

  if (read(sock, buffer, MAX_REQUEST_SIZE) < 0)
  {
    error("ERROR at recieving request");
  }

  std::string sBuffer(buffer);

  std::cout << "==== REQUEST:" << std::endl
    << (sBuffer.size() > MAX_OUTPUT_SIZE ? 
        sBuffer.substr(0, MAX_OUTPUT_SIZE) + "..."
      : sBuffer) << std::endl << std::endl;

  Request req(sBuffer);
  Response res(req);

  std::string response_text = res.getResponse();
  int response_text_size = res.getResponseSize();

  std::cout << "==== RESPONSE:" << std::endl 
    << (response_text.size() > MAX_OUTPUT_SIZE ? 
      response_text.substr(0, MAX_OUTPUT_SIZE) + "..." 
      : response_text) 
    << std::endl << std::endl;

  if (write(sock, 
      response_text.c_str(), 
      response_text_size) < 0)
  {
    error("ERROR at sending response");
  }

  std::cout << "RESPONSE COMPLETE, LISTENING..." 
    << std::endl << std::endl;
}

void runMigrations()
{
  Database::runMigrations();
}