#include "se.h"
#include <stdio.h>
#include <stdlib.h>
#include <libxml2/libxml/xmlmemory.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

// Constructor, opens the settings file.
settings::settings(char *filename) {
  	settings_file = xmlParseFile(filename);

	// Dump an error when 
	if (settings_file == NULL) {
		cout << "Error while opening settings. File: " << filename << endl;
	}
}

// Read a setting.
char* settings::get_setting(char *element, char *attribute) {
	// Read the root element out of settings_file
  	xmlNode *settings_root = NULL;
	char *return_value;

	return_value = NULL;
  	settings_root = xmlDocGetRootElement(settings_file);
  	if(!settings_root || !settings_root->name || xmlStrcmp(settings_root->name,(xmlChar *)"StarLaneSettings")) {
     		cout << "Error: Not a starlane settings document." << endl;
     		return NULL;
  	}
	// Browse through the settings file
	for (current_node = settings_root->children; current_node != NULL; current_node = current_node->next) {
		// If we find the wanted element start browsing it for attributes
		if (current_node->type == XML_ELEMENT_NODE && !xmlStrcmp(current_node->name,(const xmlChar *)element)) {
			#ifdef DEBUG
			cout << "Element found: " << element << endl;
			#endif
			for (child_node = current_node->children; child_node != NULL; child_node = child_node->next) {
				if (child_node->type == XML_ELEMENT_NODE && !xmlStrcmp(child_node->name,(const xmlChar *)attribute)) {
								#ifdef DEBUG
								cout << "Attribute found: " << attribute << endl;
								#endif
					return_value = (char *)xmlNodeGetContent(child_node);
				}
			}
		}
	}
	//Remember to send something back.
	return return_value;
}

// Destructor, closes everything nice and tidy.
settings::~settings() {
	xmlFreeDoc(settings_file);
	xmlCleanupParser();
}
