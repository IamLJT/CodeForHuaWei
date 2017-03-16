#include "deploy.h"

//你要完成的功能总入口
void deploy_server(char * topo[MAX_EDGE_NUM], int line_num, char * filename)
{
	int i, j;
	int header_info[3];
	split_item(topo[0], 3, header_info);
	
	NetWorkNode ** network = new NetWorkNode*[header_info[0] * header_info[0]];
	for (i = 0; i < header_info[0] * header_info[0]; ++i) network[i] = NULL;
	ConsumerNode ** extra_topo = new ConsumerNode*[header_info[2]];
	generate_data_structure(topo, line_num, header_info, network, extra_topo);

	std::vector<Cluster> _cluster;
	std::vector<Cons_Cluster> _cons_cluster;
	cluster_data(network, extra_topo, header_info, _cluster, _cons_cluster, line_num);

	for (i = 0; i < header_info[0]; ++i) {
		for (j = 0; j < header_info[0]; ++j) {
			if (NULL != network[i * header_info[0] + j]) {
				printf(">> [%d][%d] NetWorkNode : bandwidth %d, rent fee %d ...\n", i, j, network[i * header_info[0] + j] -> get_bandwidth(), network[i * header_info[0] + j] -> get_rent_fee());
				delete network[i * header_info[0] + j];
			}
		}
	}

	for (i = 0; i < header_info[2]; ++i) {
		printf(">> [%d] ConsumerNode : connected node %d, command %d ...\n", i, extra_topo[i] -> get_connected_node(), extra_topo[i] -> get_command());
		delete extra_topo[i];
	}

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	// write_result(topo_file, filename);
}

void split_item(char * line, int n, int * result) {
	int i = 0;

	char ** item = new char*[n];
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
		graph[network_node[0] * header[0] + network_node[1]] = new NetWorkNode(network_node[2], network_node[3]);
		graph[network_node[1] * header[0] + network_node[0]] = new NetWorkNode(network_node[2], network_node[3]);
	}
	printf("**************************************************\n");
	for (++i; i < header[1] + 4 + 1 + header[2]; ++i) {
		split_item(topo[i], 3, consumer_node);
		extra_graph[consumer_node[0]] = new ConsumerNode(consumer_node[1], consumer_node[2]);
	}
	printf("**************************************************\n");
}

void BFS(NetWorkNode **graph, std::vector<bool> graph_visit, std::vector<int> conj_point, std::set<int>& cluster, std::set<int>& cons_cluster, std::set<int>& cons_cluster_temp, int len_cluster, std::queue<int>& q, int *header_info)
{
	if (q.empty() || len_cluster == 0) return;
	int len = q.size();
	for (int i = 0; i < len; ++i) {
		int pos = q.front();
		q.pop();
		for (int j = 0; j < header_info[0]; ++j) {
			if (graph[pos * header_info[0] + j] != NULL && graph_visit[j] == false) {
				cluster.insert(j);
				q.push(j);
				if (conj_point[j] == -1)
					graph_visit[j] = true;
				else {
					cons_cluster.insert(conj_point[j]);
					cons_cluster_temp.insert(conj_point[j]);
				}
			}
		}
	}
	len_cluster--;
	BFS(graph, graph_visit, conj_point, cluster, cons_cluster, cons_cluster_temp, len_cluster, q, header_info);
}

void cluster_data(NetWorkNode **graph, ConsumerNode **extra_graph, int *header_info, std::vector<Cluster>& _cluster, std::vector<Cons_Cluster>&_cons_cluster, int line_num)
{
	using std::vector;
	using std::set;
	using std::queue;
	vector<bool> graph_visit(header_info[0], false);
	vector<int> conj_point(header_info[0], -1);
	for (int i = 0; i < header_info[2]; ++i)
		conj_point[extra_graph[i]->get_connected_node()] = i;
	NetWorkNode **graph0 = graph;
	set<int> cons_cluster;	// 存储已遍历过的消费节点
	NetWorkNode **graph_temp = new NetWorkNode*[header_info[0] * header_info[0]];
	ConsumerNode **extra_graph_temp = new ConsumerNode*[header_info[2]];
	for (int i = 0; i < header_info[2]; ++i) {
		if (cons_cluster.size() && cons_cluster.find(i) != cons_cluster.end())
			continue;
		cons_cluster.insert(i);
		set<int> cluster, cons_cluster_temp;
		queue<int> q;
		q.push(extra_graph[i]->get_connected_node());
		cluster.insert(extra_graph[i]->get_connected_node());
		cons_cluster_temp.insert(i);
		// 广度搜索
		BFS(graph0, graph_visit, conj_point, cluster, cons_cluster, cons_cluster_temp, len_cluster, q, header_info);
		// 重新存储聚类后的网点图
		for (int k = 0; k < header_info[0] * header_info[0]; ++k) graph_temp[k] = NULL;
		for (auto itra = cluster.begin(); itra != cluster.end(); ++itra) {
			for (int j = 0; j < header_info[0]; ++j) {
				graph_temp[*itra * header_info[0] + j] = graph[*itra * header_info[0] + j];
				graph_temp[j * header_info[0] + *itra] = graph[j * header_info[0] + *itra];
			}
		}
		for (int k = 0; k < header_info[2]; ++k) extra_graph_temp[k] = NULL;
		for (auto itra = cons_cluster_temp.begin(); itra != cons_cluster_temp.end(); ++itra) {
				extra_graph_temp[*itra] = extra_graph[*itra];
		}
		_cluster.push_back(Cluster(graph_temp));
		_cons_cluster.push_back(Cons_Cluster(extra_graph_temp));
	}
}