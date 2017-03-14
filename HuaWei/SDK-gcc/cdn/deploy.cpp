#include "deploy.h"

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num, char * filename)
{
	int i, j;
	int header_info[3];
	split_item(topo[0], 3, header_info);

	NetWorkNode * network[line_num * line_num];
	for (i = 0; i < line_num * line_num; ++i) network[i] = NULL;
	ConsumerNode * extra_topo[header_info[2]];
	generate_data_structure(topo, line_num, header_info, network, extra_topo);

	for (i = 0; i < line_num; ++i) {
		for (j = 0; j < line_num; ++j) {
			if (NULL != network[i * line_num + j]) {
				printf(">> [%d][%d] NetWorkNode : bandwidth %d, rent fee %d ...\n", i, j, network[i * line_num + j] -> get_bandwidth(), network[i * line_num + j] -> get_rent_fee());
				delete network[i * line_num + j];
			}
		}
	}

	for (i = 0; i < header_info[2]; ++i) {
		printf(">> [%d][%d] ConsumerNode : connected node %d, command %d ...\n", i, j, extra_topo[i] -> get_connected_node(), extra_topo[i] -> get_command());
		delete extra_topo[i];
	}

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	// write_result(topo_file, filename);
}

void split_item(char * line, int n, int * result) {
	int i = 0;

	char * item[n];
	const char * delimeter = " ";
	item[i] = strtok(line, delimeter);
	result[i] = atoi(item[i]);
	while((item[++i] = strtok(NULL, delimeter))) {
		result[i] = atoi(item[i]);
	}
}

void generate_data_structure(char* topo[MAX_EDGE_NUM], int line_num, int * header, NetWorkNode ** graph, ConsumerNode ** extra_graph) {
	int i = 4;
	int network_node[4];
	int consumer_node[3];

	printf("**************************************************\n");
	for (; i < header[1] + 4; ++i) {
		split_item(topo[i], 4, network_node);
		graph[network_node[0] * line_num + network_node[1]] = new NetWorkNode(network_node[2], network_node[3]);
	}
	printf("**************************************************\n");
	for (++i; i < header[1] + 4 + 1 + header[2]; ++i) {
		split_item(topo[i], 3, consumer_node);
		extra_graph[consumer_node[0]] = new ConsumerNode(consumer_node[1], consumer_node[2]);
	}
	printf("**************************************************\n");
}
