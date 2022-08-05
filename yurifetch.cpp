#include "api.h"
#include <ctime>
#include <curl/curl.h>
#include <iostream>
#include <pugixml.hpp>
#include <stdio.h>
#include <string>
int postCount;

pugi::xml_document doc;
pugi::xml_node posts, post;

CURL *curl;
CURLcode res;
std::string file_url, file_name, url, tag, pid;
std::string api_key = API_KEY;
char xmlname[FILENAME_MAX] = "tree.xml";
using namespace std;
void get_file(std::string, std::string);
void get_random_post();
int children_count(pugi::xml_node);
int main(int argc, char *argv[]) {
  pid = "1";
  srand((unsigned)time(0));
  if (argc > 1) {
    tag = argv[1];
    if (argc > 2) {
      pid = argv[2];
    }
  } else {
    cout << "No tags! \nUsage: yurifetch: \"tag1+tag2\" <pid> (optional) "
         << endl;
    exit(1);
  }
  if (tag == "-v") {
    cout << "yurifetch v0.1 by aryl" << endl;
    exit(0);
  }
  url = string("https://gelbooru.com/"
               "index.php?page=dapi&s=post&q=index&json=0&tags=") += tag +=
      string(API_KEY) += string(USER_ID) += string("&pid=") += pid;
  // cout << url << endl; // debug thing
  curl = curl_easy_init(); // curl init & getting/parsing the actual xml file
  get_file(url, "tree.xml");
  pugi::xml_parse_result result = doc.load_file("tree.xml");
  posts = doc.child("posts"); // get the root posts array
  postCount = children_count(posts);
  if (postCount < 1) {
    cout << "No Posts Found!";
    exit(1);
  }
  cout << postCount << " posts fetched~!" << endl;
  post = posts.child("post"); // get the actual post
  get_random_post();
  file_url = post.child_value("file_url");
  file_name = post.child_value("image");
  cout << file_name << endl;
  cout << "Getting Image..." << endl;
  get_file(file_url, file_name);
  curl_easy_cleanup(curl);
}

void get_file(std::string link, std::string outfilename) {
  if (curl) {
    FILE *file;
    file = fopen(outfilename.c_str(), "wb");
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform(curl);
    fclose(file);
  } else {
    cout << "Curl Failed!" << endl;
    exit(1);
  }
}

void get_random_post() { // get one random post out of 100
  for (int i = 0; i < (rand() % (postCount - 1)); ++i) {
    post = post.next_sibling();
  }
}
int children_count(pugi::xml_node node) {
  int n = 0;
  for (pugi::xml_node child : node.children())
    n++;
  return n;
}