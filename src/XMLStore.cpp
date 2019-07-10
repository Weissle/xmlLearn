#include"pugixml.cpp"
#include"pugixml.hpp"
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include"fts_fuzzy_match.h"
using namespace std;
void newXml();
void loadXml();
void exit_program();
void ls(const pugi::xml_node& node);
void blockOpt(pugi::xml_node &blockNode, pugi::xml_node &blockDesc);
pugi::xml_document doc;
string filePath;
int main() {
	while (true) {
		cout << "XMLStore :";
		string opt;
		cin >> opt;
		std::for_each(opt.begin(), opt.end(), [](auto &c) {
			c = towlower(c);
		});
		if (opt == "new") {
			newXml();
		}
		else if (opt == "load") {
			loadXml();
		}
					

		else if (opt == "exit") {
			doc.save_file(filePath.c_str());
			exit(0);
		}
	}
	return 0;
}

void newXml() {

	cout << "Please input xml file location : " ;
	cin >> filePath;
	string fileName;
	fileName = filePath.substr(filePath.find_last_of('/') + 1);
	cout << "Please descripe this xml" << endl;
	auto root = doc.append_child("Root");
	auto descriptionNode = root.append_child("Description");
	auto dataNode = root.append_child("Data");
	cout << "How many block it has : ";
	int blockNum;
	cin >> blockNum;
	for (auto i = 0; i < blockNum; ++i) {
		cout << "BLOCK " << i + 1 << " Name: ";
		string blockName;
		cin >> blockName;
		auto blockNode = descriptionNode.append_child(blockName.c_str());
		dataNode.append_child(blockName.c_str());
		cout << "Attr Num : ";	
		int attrNum = 0;
		cin >> attrNum;
		for (auto j = 0; j < attrNum; ++j) {
			string attrName;
			cin >> attrName;
			blockNode.append_child(attrName.c_str());
		}
	}
	doc.save_file(filePath.c_str());

	//	doc.save_file(filePath.c_str());
}
void loadXml() {

	cout << "Please input xml file location : ";
	cin >> filePath;
	string fileName;
	fileName = filePath.substr(filePath.find_last_of('/') + 1);

	pugi::xml_parse_result result = doc.load_file(filePath.c_str());
	if (result == false) {
		cout << result.description() << endl;
		return;
	}
	
	auto root = doc.child("Root");
	auto descNode = root.child("Description");
	auto dataNode = root.child("Data");

	while (true) {
		cout << fileName << ": ";
		string opt;
		cin >> opt;
		std::for_each(opt.begin(), opt.end(), [](auto &c) {
			c = towlower(c);
		});
		if (opt == "ls") {
			ls(descNode);
		}
		else if (opt == "find") {
			string name;
			cin >> name;
			auto tempNode = dataNode.child(name.c_str());
			if (tempNode) {
				cout << name << " ";
				for (auto &attr : tempNode.attributes()) {
					cout << attr.as_string() << " ";
				}
				cout << endl;
			}
		}
		else if (opt == "inter") {
			string blockName;
			cin >> blockName;
			auto dataBlockNode = dataNode.child(blockName.c_str());
			blockOpt(dataBlockNode,descNode.child(blockName.c_str()));
		}
		else if (opt == "exit") {
			doc.save_file(filePath.c_str());
			exit(0);
		}
	}


}
void blockOpt(pugi::xml_node &blockNode,pugi::xml_node &blockDesc) {
	string blockName = blockNode.name();

	while (true) {
		cout << blockName << " : " << endl;
		string opt;
		cin >> opt;
		std::for_each(opt.begin(), opt.end(), [](auto &c) {
			c = towlower(c);
		});
		if (opt == "ls") {
			ls(blockNode);
		}
		else if (opt == "insert") {
			string nodeName;
			cin >> nodeName;
			auto tempNode = blockNode.append_child(nodeName.c_str());
			for (auto &des : blockDesc) {
				cout << des.name() << " :";
				string input;
				cin >> input;
				tempNode.append_attribute(des.name()) = input.c_str();

			}
		}
		else if (opt == "find") {
			string nodeName;
			cin >> nodeName;

			auto &tempNode = blockNode.child(nodeName.c_str());
			if (tempNode) {
				cout << tempNode.name() << " ";
				for (auto &attr : blockNode.child(nodeName.c_str()).attributes()) {
					cout << attr.as_string() << "\t ";
				}
				cout << endl;
			}
		}
		else if (opt == "delete") {
			string nodeName;
			cin >> nodeName;
			blockNode.remove_child(nodeName.c_str());
		}
		else if (opt == "exit") {
			exit_program();
		}
	}
	
}

void ls(const pugi::xml_node& node) {
	for (const auto& child : node.children()) {
		cout << child.name() << '\t';
		for (const auto& attr : child.attributes()) {
			cout << attr.as_string() << '\t';
		}
		cout << endl;
	}
}
void exit_program() {
	doc.save_file(filePath.c_str());
	exit(0);
}