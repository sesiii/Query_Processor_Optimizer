
//
//new
#include "parser.h"
#include "parser.tab.h"
#include "optimizer.h"

extern void yy_scan_string(const char *str);

Node *root = NULL;

Node *new_node(char *op, char *arg1, char *arg2) {
    Node *n = malloc(sizeof(Node));
    n->operation = op ? strdup(op) : NULL;
    n->arg1 = arg1 ? strdup(arg1) : NULL;
    n->arg2 = arg2 ? strdup(arg2) : NULL;
    n->child = NULL;
    n->next = NULL;
    return n;
}
void print_tree(Node *node, int depth) {
    if (!node) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) printf("  ");
    
    // Print current node
    if (node->arg1 && node->arg2)
        printf("%s(%s AS %s)\n", node->operation, node->arg1, node->arg2);
    else if (node->arg1)
        printf("%s(%s)\n", node->operation, node->arg1);
    else
        printf("%s\n", node->operation);
    
    // Recursively print all children (assuming child is the left subtree)
    if (node->child) {
        print_tree(node->child, depth + 1);
    }
    
    // Print siblings (assuming next is for additional children or nodes at the same level)
    if (node->next) {
        print_tree(node->next, depth + 1);  // Indent siblings as children
    }
}
int main() {
    FILE *file = fopen("query.sql", "r");
    if (!file) {
        perror("Failed to open query.sql");
        return 1;
    }

    char *line = NULL;void print_tree(Node *node, int depth) {
        if (!node) return;
        
        // Print indentation
        for (int i = 0; i < depth; i++) printf("  ");
        
        // Print current node
        if (node->arg1 && node->arg2)
            printf("%s(%s AS %s)\n", node->operation, node->arg1, node->arg2);
        else if (node->arg1)
            printf("%s(%s)\n", node->operation, node->arg1);
        else
            printf("%s\n", node->operation);
        
        // Handle special case for table with join next sibling
        if (node->operation && strcmp(node->operation, "table") == 0 && 
            node->next && node->next->operation && strcmp(node->next->operation, "⨝") == 0) {
            
            // Print join node
            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("%s(%s)\n", node->next->operation, node->next->arg1);
            
            // Print left table (current table)
            for (int i = 0; i < depth + 2; i++) printf("  ");
            printf("%s(%s)\n", node->operation, node->arg1);
            
            // Print right table (child of current table)
            if (node->child) {
                for (int i = 0; i < depth + 2; i++) printf("  ");
                printf("%s(%s)\n", node->child->operation, node->child->arg1);
            }
            
            // Skip normal child processing
            if (node->next && node->next->next) {
                print_tree(node->next->next, depth);
            }
        } else {
            // Normal processing for other nodes
            if (node->child) {
                print_tree(node->child, depth + 1);
            }
            if (node->next) {
                print_tree(node->next, depth);
            }
        }
    }
    size_t len = 0;
    ssize_t read;

    if ((read = getline(&line, &len, file)) != -1) {
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        printf("Parsing query: %s\n", line);
        yy_scan_string(line);
    } else {
        printf("No query found in query.sql\n");
        free(line);
        fclose(file);
        return 1;
    }

    free(line);
    fclose(file);
    yyparse();

    if (root) {
        printf("\nOriginal Abstract Syntax Tree:\n");
        print_tree(root, 0);
        
        // Optimize the query
        root = optimize_query(root);
    } else {
        printf("No AST generated.\n");
    }
    return 0;
}