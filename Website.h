#pragma once 
#include <Arduino.h>
#include <WebServer.h>

extern WebServer server;

void handle_OnConnect(float waterlevel, bool LED, String waterstatus);
void handle_NotFound();

String SendHTML(float waterlevel, bool LED, String waterstatus);