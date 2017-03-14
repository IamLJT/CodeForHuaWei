#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
public:
    Node() {}
	~Node() {}
};

struct NetWorkNode : public Node {
public:
    NetWorkNode(int m, int n) {
		_bandwidth = m;
		_rent_fee = n; 
		printf("[+] New Edege Between NetWorkNode And NetWorkNode ...\n");
	}
	~NetWorkNode() {}

	int get_bandwidth() {
		return _bandwidth;
	}
	int get_rent_fee() {
		return _rent_fee;
	}

private:
	int _bandwidth;
	int _rent_fee;
};

struct ConsumerNode : public Node {
public:
    ConsumerNode(int m, int n) {
		_connected_node = m;
		_command = n;
		printf("[+] New Edege Between ConsumerNode And NetWorkNode ...\n");
	}
	~ConsumerNode() {}

	int get_connected_node() {
		return _connected_node;
	}
	int get_command() {
		return _command;
	}
private:
	int _connected_node;
	int _command;
};

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num, char * filename);
void split_item(char * line, int n, int * result);
void generate_data_structure(char* topo[MAX_EDGE_NUM], int line_num, int * header, NetWorkNode ** graph, ConsumerNode ** extra_graph);

#endif
