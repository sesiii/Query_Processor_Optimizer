// #include "optimizer.h"
// #include "stats.h"
// #include <math.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// // Flag to enable/disable optimizations
// int enable_selection_pushdown = 1;
// int enable_projection_pushdown = 1;
// int enable_join_reordering = 0;
// int debugkaru = 0;

// // Node* optimize_query(Node *root) {
// //     if (!root) return NULL;
    
// //     printf("\nOptimizing query...\n");
    
// //     init_stats();
    
// //     printf("\nOriginal Execution Plan:\n");
// //     CostMetrics original_cost = estimate_cost(root);
// //     print_execution_plan(root, "Original Plan");
    
// //     Node *optimized_root = root;
// //     if (enable_selection_pushdown) {
// //         printf("\nApplying selection push-down...\n");
// //         optimized_root = push_down_selections(optimized_root);
// //     }

// //     if (enable_projection_pushdown) {
// //         printf("\nApplying projection push-down...\n");
// //         optimized_root = push_down_projections(optimized_root);
// //     }
    
// //     printf("\nOptimized Execution Plan:\n");
// //     CostMetrics optimized_cost = estimate_cost(optimized_root);
// //     print_execution_plan(optimized_root, "Optimized Plan");
    
// //     printf("\nCost Comparison:\n");
// //     printf("Metric          | Original      | Optimized     | Change\n");
// //     printf("----------------|---------------|---------------|----------------\n");
// //     printf("Result Size     | %-13d | %-13d | %s%d (%.1f%%)\n",
// //            original_cost.result_size, optimized_cost.result_size,
// //            (optimized_cost.result_size <= original_cost.result_size) ? "-" : "+",
// //            abs(optimized_cost.result_size - original_cost.result_size),
// //            original_cost.result_size ? 
// //            (double)abs(optimized_cost.result_size - original_cost.result_size) / original_cost.result_size * 100 : 0.0);
// //     printf("I/O Cost        | %-13.1f | %-13.1f | %s%.1f (%.1f%%)\n",
// //            original_cost.io_cost, optimized_cost.io_cost,
// //            (optimized_cost.io_cost <= original_cost.io_cost) ? "-" : "+",
// //            fabs(optimized_cost.io_cost - original_cost.io_cost),
// //            original_cost.io_cost ? 
// //            fabs(optimized_cost.io_cost - original_cost.io_cost) / original_cost.io_cost * 100 : 0.0);
// //     printf("CPU Cost        | %-13.1f | %-13.1f | %s%.1f (%.1f%%)\n",
// //            original_cost.cpu_cost, optimized_cost.cpu_cost,
// //            (optimized_cost.cpu_cost <= original_cost.cpu_cost) ? "-" : "+",
// //            fabs(optimized_cost.cpu_cost - original_cost.cpu_cost),
// //            original_cost.cpu_cost ? 
// //            fabs(optimized_cost.cpu_cost - original_cost.cpu_cost) / original_cost.cpu_cost * 100 : 0.0);
    
// //     return optimized_root;
// // }

// // Helper function to duplicate a node (deep copy)
// Node* duplicate_node(Node *node) {
//     if (!node) return NULL;
//     Node *new_ = new_node(node->operation, node->arg1, node->arg2);
//     new_->child = duplicate_node(node->child);
//     new_->next = duplicate_node(node->next);
//     return new_;
// }

// // Helper function to free a node
// void free_node(Node *node) {
//     if (!node) return;
//     free_node(node->child);
//     free_node(node->next);
//     if (node->operation) free(node->operation);
//     if (node->arg1) free(node->arg1);
//     if (node->arg2) free(node->arg2);
//     free(node);
// }

// Node* optimize_query(Node *root) {
//     if (!root) return NULL;
    
//     printf("\nOptimizing query...\n");
    
//     init_stats();
    
//     // Compute original plan cost
//     printf("\nOriginal Execution Plan:\n");
//     CostMetrics original_cost = estimate_cost(root);
//     print_execution_plan(root, "Original Plan");
    
//     // Store the original plan for multiple optimizations
//     Node *original_root = duplicate_node(root);
    
//     // Plan 1: Selection pushdown
//     Node *selection_optimized = duplicate_node(original_root);
//     CostMetrics selection_cost = {0, 0.0, 0.0};
//     if (enable_selection_pushdown) {
//         printf("\nApplying selection push-down...\n");
//         selection_optimized = push_down_selections(selection_optimized);
//         selection_cost = estimate_cost(selection_optimized);
//         print_execution_plan(selection_optimized, "Selection Pushdown Plan");
//     } else {
//         selection_cost = original_cost; // No optimization applied
//     }
    
//     // Plan 2: Projection pushdown (reset to original)
//     Node *projection_optimized = duplicate_node(original_root);
//     CostMetrics projection_cost = {0, 0.0, 0.0};
//     if (enable_projection_pushdown) {
//         printf("\nApplying projection push-down...\n");
//         projection_optimized = push_down_projections(projection_optimized);
//         projection_cost = estimate_cost(projection_optimized);
//         print_execution_plan(projection_optimized, "Projection Pushdown Plan");
//     } else {
//         projection_cost = original_cost; // No optimization applied
//     }
    
//     // Determine the best plan based on total cost (I/O + CPU)
//     double original_total = original_cost.io_cost ;
//     double selection_total = selection_cost.io_cost ;
//     double projection_total = projection_cost.io_cost ;
    
//     Node *best_plan = root; // Default to original
//     CostMetrics best_cost = original_cost;
//     const char *best_plan_name = "Original";
//     double best_total = original_total;
    
//     if (selection_total <= best_total) {
//         best_plan = selection_optimized;
//         best_cost = selection_cost;
//         best_plan_name = "Selection Pushdown";
//         best_total = selection_total;
//     } else {
//         free_node(selection_optimized); // Free if not selected
//     }
    
//     if (projection_total <= best_total) {
//         if (best_plan != root) free_node(best_plan); // Free previous best if not original
//         best_plan = projection_optimized;
//         best_cost = projection_cost;
//         best_plan_name = "Projection Pushdown";
//         best_total = projection_total;
//     } else {
//         free_node(projection_optimized); // Free if not selected
//     }
    
//     // Free the duplicated original if not selected
//     if (best_plan != root) {
//         free_node(original_root);
//     }
    
//     // Print cost comparison
//     printf("\nCost Comparison:\n");
//     printf("Metric          | Original      | Selection pushdown     | Projection pushdown    |\n");
//     printf("----------------|---------------|------------------------|------------------------|\n");
//     printf("Result Size     | %-13d | %-22d | %-22d | \n",
//            original_cost.result_size, selection_cost.result_size, projection_cost.result_size);
//     printf("I/O Cost        | %-13.1f | %-22f | %-22f | \n",
//            original_cost.io_cost, selection_cost.io_cost, projection_cost.io_cost);
//     // printf("CPU Cost        | %-13.1f | %-13.1f | %-13.1f | %s\n",
//     //        original_cost.cpu_cost, selection_cost.cpu_cost, projection_cost.cpu_cost,
//     //        best_plan_name);
//     printf("Total Cost      | %-13.1f | %-22f | %-22f | \n",
//            original_total, selection_total, projection_total);
    
//     // Print the best plan
//     printf("\nSelected Best Execution Plan (%s):\n", best_plan_name);
//     print_execution_plan(best_plan, "Best Plan");
    
//     return best_plan;
// }

// // ... (rest of the file remains unchanged)
// void extract_table_column(const char *expr, char **table, char **column) {
//     char *expr_copy = strdup(expr);
//     char *start = expr_copy;
//     while (isspace(*start)) start++;
//     char *end = start + strlen(start) - 1;
//     while (end > start && isspace(*end)) *end-- = '\0';
    
//     char *dot = strchr(start, '.');
//     if (dot) {
//         int table_len = dot - start;
//         *table = malloc(table_len + 1);
//         strncpy(*table, start, table_len);
//         (*table)[table_len] = '\0';
//         *column = strdup(dot + 1);
//     } else {
//         *table = NULL;
//         *column = strdup(start);
//     }
//     free(expr_copy);
// }

// void extract_condition_components(const char *condition, char **table, char **column, char **op, int *value) {
//     char condition_copy[100];
//     strcpy(condition_copy, condition);
    
//     char *token = strtok(condition_copy, " ");
//     if (token) {
//         extract_table_column(token, table, column);
//         token = strtok(NULL, " ");
//         if (token) {
//             *op = strdup(token);
//             token = strtok(NULL, " ");
//             if (token) {
//                 *value = atoi(token);
//             } else {
//                 *value = 0;
//             }
//         } else {
//             *op = NULL;
//             *value = 0;
//         }
//     } else {
//         *table = *column = *op = NULL;
//         *value = 0;
//     }
// }

// int can_push_to_table(const char *condition, const char *table_name, Node *context) {
//     char *table = NULL, *column = NULL, *op = NULL;
//     int value = 0;
//     extract_condition_components(condition, &table, &column, &op, &value);

//     if (debugkaru) {
//         printf("Checking if '%s' can push to '%s': table=%s, column=%s\n", 
//                condition, table_name, table ? table : "NULL", column ? column : "NULL");
//     }

//     int result = 0;
//     if (table && column) {
//         result = strcmp(table, table_name) == 0 && get_column_stats(table_name, column) != NULL;
//     } else if (column) {
//         ColumnStats *stats = get_column_stats(table_name, column);
//         if (stats) {
//             result = 1;
//             if (debugkaru) printf("push down possible: column %s exists in table %s\n", column, table_name);
//         }
//     }
    
//     if (debugkaru) printf("Result: %d\n", result);
//     if (table) free(table);
//     if (column) free(column);
//     if (op) free(op);
//     return result;
// }

// Node* push_down_selections(Node *node) {
//     if (!node) return NULL;
//     if (debugkaru) printf("Pushing down selections...%s\n", node->operation ? node->operation : "NULL");
    
//     if (node->operation && strcmp(node->operation, "σ") == 0) {
//         Node *child = node->child;
//         if (!child) return node;
        
//         if (child->operation && strcmp(child->operation, "table") == 0 && 
//             child->next && child->next->operation && strcmp(child->next->operation, "⨝") == 0) {
            
//             Node *left_table = child;
//             Node *join_node = child->next;
//             Node *right_table = NULL;
            
//             if (left_table->child && left_table->child->operation && 
//                 strcmp(left_table->child->operation, "table") == 0) {
//                 right_table = left_table->child;
//             }
            
//             if (right_table && can_push_to_table(node->arg1, right_table->arg1, node)) {
//                 if (debugkaru) printf("Pushing condition '%s' down to table '%s'\n", node->arg1, right_table->arg1);
                
//                 Node *new_selection = new_node("σ", strdup(node->arg1), NULL);
//                 new_selection->child = right_table;
                
//                 left_table->child = NULL;
                
//                 join_node->child = left_table;
//                 left_table->next = new_selection;
                
//                 node->child = NULL;
//                 free(node->operation);
//                 free(node->arg1);
//                 free(node);
                
//                 return join_node;
//             } 
//             else if (can_push_to_table(node->arg1, left_table->arg1, node)) {
//                 if (debugkaru) printf("Pushing condition '%s' down to table '%s'\n", node->arg1, left_table->arg1);
                
//                 Node *new_selection = new_node("σ", strdup(node->arg1), NULL);
//                 new_selection->child = left_table;
                
//                 left_table->child = NULL;
                
//                 join_node->child = new_selection;
//                 new_selection->next = right_table;
                
//                 node->child = NULL;
//                 free(node->operation);
//                 free(node->arg1);
//                 free(node);
                
//                 return join_node;
//             }
//         }
//     }
    
//     if (node->child) node->child = push_down_selections(node->child);
//     if (node->next) node->next = push_down_selections(node->next);
    
//     return node;
// }

// void parse_join_condition(const char *condition, char **left_table, char **left_col, 
//                          char **right_table, char **right_col) {
//     char condition_copy[100];
//     strcpy(condition_copy, condition);
    
//     char *left_side = strtok(condition_copy, "=");
//     char *right_side = strtok(NULL, "=");
    
//     if (left_side && right_side) {
//         char *left_clean = left_side;
//         char *right_clean = right_side;
//         while (isspace(*left_clean)) left_clean++;
//         while (isspace(*right_clean)) right_clean++;
//         char *left_end = left_clean + strlen(left_clean) - 1;
//         char *right_end = right_clean + strlen(right_clean) - 1;
//         while (left_end > left_clean && isspace(*left_end)) *left_end-- = '\0';
//         while (right_end > right_clean && isspace(*right_end)) *right_end-- = '\0';
        
//         if (debugkaru) printf("Parsing join: left='%s', right='%s'\n", left_clean, right_clean);
//         extract_table_column(left_clean, left_table, left_col);
//         extract_table_column(right_clean, right_table, right_col);
//     } else {
//         *left_table = *left_col = *right_table = *right_col = NULL;
//     }
// }

// double estimate_join_cost(const char *left_table, const char *right_table, const char *condition) {
//     TableStats *left_stats = get_table_stats(left_table);
//     TableStats *right_stats = get_table_stats(right_table);
    
//     if (!left_stats || !right_stats) {
//         if (debugkaru) printf("No stats for %s or %s\n", left_table, right_table);
//         return 1000.0;
//     }
    
//     char *left_table_name, *left_col, *right_table_name, *right_col;
//     parse_join_condition(condition, &left_table_name, &left_col, &right_table_name, &right_col);
    
//     double selectivity = 1.0 / fmax(left_stats->row_count, right_stats->row_count);
//     if (left_table_name && right_table_name && left_col && right_col) {
//         double sel = calculate_join_selectivity(left_table_name, left_col, right_table_name, right_col);
//         if (sel > 0.0 && sel < 1.0) {
//             selectivity = sel;
//             if (debugkaru) printf("Join selectivity for %s.%s = %s.%s: %f\n", 
//                                  left_table_name, left_col, right_table_name, right_col, selectivity);
//         }
//     }
    
//     double result_size = left_stats->row_count * right_stats->row_count * selectivity;
    
//     if (left_table_name) free(left_table_name);
//     if (left_col) free(left_col);
//     if (right_table_name) free(right_table_name);
//     if (right_col) free(right_col);
    
//     return result_size;
// }

// JoinOrderNode* find_optimal_join_order(Node *join_node) {
//     if (debugkaru) printf("Finding optimal join order (simplified implementation)\n");
//     return NULL;
// }

// Node* reorder_joins(Node *node) {
//     if (!node) return NULL;
    
//     if (node->operation && strcmp(node->operation, "⨝") == 0) {
//         if (debugkaru) printf("Join reordering would be applied here\n");
//         if (node->child) node->child = reorder_joins(node->child);
//         if (node->next) node->next = reorder_joins(node->next);
//     } else {
//         if (node->child) node->child = reorder_joins(node->child);
//         if (node->next) node->next = reorder_joins(node->next);
//     }
    
//     return node;
// }

// static int is_table_scan(Node *node) {
//     return node && node->operation && strcmp(node->operation, "table") == 0;
// }

// static double get_condition_selectivity(Node *node) {
//     if (!node || !node->arg1) return 0.1; // Default selectivity
    
//     char *table = NULL, *column = NULL, *op = NULL;
//     int value = 0;
//     extract_condition_components(node->arg1, &table, &column, &op, &value);
    
//     double selectivity = 0.1; // Default
//     if (table && column && op) {
//         selectivity = calculate_condition_selectivity(table, column, op, value);
//     } else if (column && op) {
//         // Try to find the table in child nodes
//         Node *current = node->child;
//         while (current) {
//             if (current->operation && strcmp(current->operation, "table") == 0) {
//                 if (get_column_stats(current->arg1, column)) {
//                     selectivity = calculate_condition_selectivity(current->arg1, column, op, value);
//                     break;
//                 }
//             }
//             current = current->child ? current->child : current->next;
//         }
//     }
    
//     if (table) free(table);
//     if (column) free(column);
//     if (op) free(op);
    
//     return selectivity;
// }

// // Helper function to get join selectivity
// static double get_join_selectivity(Node *node) {
//     if (!node || !node->arg1) return 0.0001; // Default selectivity
    
//     char *left_table = NULL, *left_col = NULL;
//     char *right_table = NULL, *right_col = NULL;
//     parse_join_condition(node->arg1, &left_table, &left_col, &right_table, &right_col);
    
//     double selectivity = 0.0001; // Default
//     if (left_table && right_table && left_col && right_col) {
//         selectivity = calculate_join_selectivity(left_table, left_col, right_table, right_col);
//     }
    
//     if (left_table) free(left_table);
//     if (left_col) free(left_col);
//     if (right_table) free(right_table);
//     if (right_col) free(right_col);
    
//     return selectivity;
// }
// CostMetrics estimate_cost(Node *node) {
//     CostMetrics metrics = {0, 0.0, 0.0};
//     if (!node) return metrics;

//     if (strcmp(node->operation, "table") == 0) {
//         TableStats *stats = get_table_stats(node->arg1);
//         metrics.result_size = stats ? stats->row_count : 1000;
//         metrics.io_cost = metrics.result_size * 1.0;  // 1 unit per row
//         // metrics.cpu_cost = metrics.result_size * 0.1; // 0.1 units per row
//         return metrics;
//     }

//     if (strcmp(node->operation, "σ") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         double selectivity = get_condition_selectivity(node);
        
//         metrics.result_size = (int)(child.result_size * selectivity);
//         metrics.io_cost = child.io_cost;
//         // metrics.cpu_cost = child.cpu_cost + (child.result_size * 0.2);
//         return metrics;
//     }

//     if (strcmp(node->operation, "⨝") == 0) {
//         if (!node->child || !node->child->next) return metrics;
        
//         CostMetrics left = estimate_cost(node->child);
//         CostMetrics right = estimate_cost(node->child->next);
//         double selectivity = get_join_selectivity(node);
        
//         metrics.result_size = (int)(left.result_size * right.result_size * selectivity);
//         metrics.io_cost = left.io_cost + right.io_cost;
//         // metrics.cpu_cost = left.cpu_cost + right.cpu_cost + (metrics.result_size * 0.1);
//         return metrics;
//     }

//     if (strcmp(node->operation, "π") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         metrics.result_size = child.result_size;
        
//         if (is_table_scan(node->child)) {
//             metrics.io_cost = child.io_cost * 0.5; // 50% I/O reduction
//         } else {
//             metrics.io_cost = child.io_cost;
//         }
        
//         // metrics.cpu_cost = child.cpu_cost + (child.result_size * 0.05);
//         return metrics;
//     }

//     return metrics;
// }
// void print_execution_plan_recursive(Node *node, int depth) {
//     if (!node) return;
    
//     for (int i = 0; i < depth; i++) printf("  ");
    
//     CostMetrics metrics = estimate_cost(node);
    
//     if (strcmp(node->operation, "table") == 0) {
//         printf("table(%s) [size=%d, io=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.io_cost);
//     }
//     else if (strcmp(node->operation, "σ") == 0) {
//         printf("σ(%s) [size=%d, io=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.io_cost );
//     }
//     else if (strcmp(node->operation, "⨝") == 0) {
//         printf("⨝(%s) [size=%d, io=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.io_cost);
//     }
//     else if (strcmp(node->operation, "π") == 0) {
//         printf("π(%s) [size=%d, io=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.io_cost);
//     }
//     else {
//         printf("%s %s %s [size=%d, io=%.1f]\n", 
//                node->operation, 
//                node->arg1 ? node->arg1 : "", 
//                node->arg2 ? node->arg2 : "", 
//                metrics.result_size, metrics.io_cost);
//     }
    
//     print_execution_plan_recursive(node->child, depth + 1);
//     print_execution_plan_recursive(node->next, depth);
// }

// void print_execution_plan(Node *node, const char *title) {
//     printf("--- %s ---\n", title);
//     print_execution_plan_recursive(node, 0);
// }

// Node* push_down_projections(Node *node) {
//     if (!node) {
//         if (debugkaru) printf("\n[DEBUG] Node is NULL, returning NULL\n");
//         return NULL;
//     }
    
//     if (debugkaru) printf("\n[DEBUG] ENTER push_down_projections for node type: %s, arg1: %s\n", 
//            node->operation ? node->operation : "NULL", 
//            node->arg1 ? node->arg1 : "NULL");
    
//     if (node->operation && strcmp(node->operation, "π") == 0) {
//         if (debugkaru) printf("\n[DEBUG] Found projection node with columns: %s\n", node->arg1);
        
//         Node *child = node->child;
//         if (!child) {
//             if (debugkaru) printf("\n[DEBUG] Projection has no child, returning node\n");
//             return node;
//         }
        
//         if (debugkaru) printf("\n[DEBUG] Child operation: %s\n", child->operation ? child->operation : "NULL");
        
//         if (child->operation && strcmp(child->operation, "σ") == 0) {
//             if (debugkaru) printf("\n[DEBUG] Case 1: Push projection through selection\n");
            
//             if (debugkaru) printf("\n[DEBUG] Selection condition: %s\n", child->arg1 ? child->arg1 : "NULL");
            
//             Node *new_projection = new_node("π", strdup(node->arg1), NULL);
//             if (debugkaru) printf("\n[DEBUG] Created new_ projection with columns: %s\n", new_projection->arg1);
            
//             new_projection->child = child->child;
//             if (debugkaru) printf("\n[DEBUG] Connected new_ projection to selection's child\n");
            
//             child->child = new_projection;
//             if (debugkaru) printf("\n[DEBUG] Put new_ projection under selection\n");
            
//             if (debugkaru) printf("\n[DEBUG] Returning selection node as new_ root\n");
//             Node *result = child;
//             node->child = NULL;
//             free(node->operation);
//             free(node->arg1);
//             free(node);
            
//             if (debugkaru) printf("\n[DEBUG] Recursively optimizing projection under selection\n");
//             result->child = push_down_projections(result->child);
            
//             if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with selection root\n");
//             return result;
//         }
//         else if (child->operation && strcmp(child->operation, "table") == 0 && 
//                  child->next && child->next->operation && strcmp(child->next->operation, "⨝") == 0) {
            
//             if (debugkaru) printf("\n[DEBUG] Case 2: Push projection through join\n");
            
//             Node *left_table = child;
//             Node *join_node = child->next;
            
//             if (debugkaru) printf("\n[DEBUG] Left table: %s\n", left_table->arg1);
//             if (debugkaru) printf("\n[DEBUG] Join condition: %s\n", join_node->arg1);
            
//             Node *right_table = NULL;
//             if (left_table->child && left_table->child->operation && 
//                 strcmp(left_table->child->operation, "table") == 0) {
//                 right_table = left_table->child;
//                 if (debugkaru) printf("\n[DEBUG] Found right table as child of left table: %s\n", right_table->arg1);
//             } else {
//                 if (debugkaru) printf("\n[DEBUG] Could not find right table, returning original node\n");
//                 if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with original node\n");
//                 return node;
//             }
            
//             if (debugkaru) printf("\n[DEBUG] Parsing projection columns: %s\n", node->arg1);
//             char *columns = strdup(node->arg1);
//             char *column_list[100];
//             int column_count = 0;
            
//             char *token = strtok(columns, ",");
//             while (token) {
//                 while (*token == ' ') token++;
//                 column_list[column_count] = strdup(token);
//                 if (debugkaru) printf("\n[DEBUG] Column %d: %s\n", column_count, column_list[column_count]);
//                 column_count++;
//                 token = strtok(NULL, ",");
//             }
            
//             if (debugkaru) printf("\n[DEBUG] Parsing join condition: %s\n", join_node->arg1);
//             char *left_table_name = NULL, *left_col = NULL;
//             char *right_table_name = NULL, *right_col = NULL;
//             parse_join_condition(join_node->arg1, &left_table_name, &left_col, 
//                                &right_table_name, &right_col);
            
//             if (debugkaru) printf("\n[DEBUG] Join parsed: %s.%s = %s.%s\n", 
//                    left_table_name ? left_table_name : "NULL", 
//                    left_col ? left_col : "NULL", 
//                    right_table_name ? right_table_name : "NULL", 
//                    right_col ? right_col : "NULL");
            
//             char left_columns[512] = "";
//             char right_columns[512] = "";
//             int left_has_columns = 0;
//             int right_has_columns = 0;
            
//             if (debugkaru) printf("\n[DEBUG] Sorting columns by table\n");
//             for (int i = 0; i < column_count; i++) {
//                 char *table = NULL, *column = NULL;
//                 extract_table_column(column_list[i], &table, &column);
                
//                 if (debugkaru) printf("\n[DEBUG] Column %d: table=%s, column=%s\n", 
//                        i, table ? table : "NULL", column ? column : "NULL");
                
//                 if (table) {
//                     if (strcmp(table, left_table->arg1) == 0) {
//                         if (left_has_columns) strcat(left_columns, ",");
//                         strcat(left_columns, column_list[i]);
//                         left_has_columns = 1;
//                         if (debugkaru) printf("\n[DEBUG] Added to left columns: %s\n", column_list[i]);
//                     }
//                     else if (strcmp(table, right_table->arg1) == 0) {
//                         if (right_has_columns) strcat(right_columns, ",");
//                         strcat(right_columns, column_list[i]);
//                         right_has_columns = 1;
//                         if (debugkaru) printf("\n[DEBUG] Added to right columns: %s\n", column_list[i]);
//                     }
//                 }
                
//                 if (table) free(table);
//                 if (column) free(column);
//             }
            
//             if (left_table_name && left_col) {
//                 char full_col[100];
//                 sprintf(full_col, "%s.%s", left_table_name, left_col);
//                 if (!strstr(left_columns, full_col)) {
//                     if (left_has_columns) strcat(left_columns, ",");
//                     strcat(left_columns, full_col);
//                     left_has_columns = 1;
//                     if (debugkaru) printf("\n[DEBUG] Added join column to left: %s\n", full_col);
//                 }
//             }
            
//             if (right_table_name && right_col) {
//                 char full_col[100];
//                 sprintf(full_col, "%s.%s", right_table_name, right_col);
//                 if (!strstr(right_columns, full_col)) {
//                     if (right_has_columns) strcat(right_columns, ",");
//                     strcat(right_columns, full_col);
//                     right_has_columns = 1;
//                     if (debugkaru) printf("\n[DEBUG] Added join column to right: %s\n", full_col);
//                 }
//             }
            
//             if (debugkaru) printf("\n[DEBUG] Final left columns: %s\n", left_columns);
//             if (debugkaru) printf("\n[DEBUG] Final right columns: %s\n", right_columns);
            
//             Node *new_left_table = new_node("table", strdup(left_table->arg1), NULL);
//             Node *new_right_table = new_node("table", strdup(right_table->arg1), NULL);
            
//             Node *left_projection = NULL;
//             if (left_has_columns) {
//                 left_projection = new_node("π", strdup(left_columns), NULL);
//                 left_projection->child = new_left_table;
//                 if (debugkaru) printf("\n[DEBUG] Created left projection: π(%s)\n", left_columns);
//             } else {
//                 left_projection = new_left_table;
//                 if (debugkaru) printf("\n[DEBUG] No left projection needed\n");
//             }
            
//             Node *right_projection = NULL;
//             if (right_has_columns) {
//                 right_projection = new_node("π", strdup(right_columns), NULL);
//                 right_projection->child = new_right_table;
//                 if (debugkaru) printf("\n[DEBUG] Created right projection: π(%s)\n", right_columns);
//             } else {
//                 right_projection = new_right_table;
//                 if (debugkaru) printf("\n[DEBUG] No right projection needed\n");
//             }
            
//             Node *new_join = new_node("⨝", strdup(join_node->arg1), NULL);
            
//             if (debugkaru) printf("\n[DEBUG] Rebuilding tree with clean structure\n");
            
//             new_join->child = left_projection;
//             left_projection->next = right_projection;
            
//             Node *top_projection = new_node("π", strdup(node->arg1), NULL);
//             top_projection->child = new_join;
            
//             for (int i = 0; i < column_count; i++) {
//                 free(column_list[i]);
//             }
//             free(columns);
//             if (left_table_name) free(left_table_name);
//             if (left_col) free(left_col); 
//             if (right_table_name) free(right_table_name);
//             if (right_col) free(right_col);
            
//             node->child = NULL;
//             free(node->operation);
//             free(node->arg1);
//             free(node);
            
//             if (debugkaru) printf("\n[DEBUG] Tree rebuilt with clean structure, returning new_ top projection\n");
//             if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with rebuilt tree\n");
//             return top_projection;
//         }
//     }
    
//     if (node->child) {
//         if (debugkaru) printf("\n[DEBUG] Recursively processing child of %s\n", node->operation);
//         node->child = push_down_projections(node->child);
//     }
//     if (node->next) {
//         if (debugkaru) printf("\n[DEBUG] Recursively processing next of %s\n", node->operation);
//         node->next = push_down_projections(node->next);
//     }
    
//     if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections returning node type: %s\n", 
//            node->operation ? node->operation : "NULL");
//     return node;
// }

// void print_ast(Node *node, int depth) {
//     if (!node) return;
    
//     for (int i = 0; i < depth; i++) {
//         printf("  ");
//     }
    
//     if (node->operation) {
//         if (node->arg1) {
//             printf("%s(%s)\n", node->operation, node->arg1);
//         } else {
//             printf("%s\n", node->operation);
//         }
//     } else {
//         printf("NULL_NODE\n");
//     }
    
//     if (node->child) {
//         print_ast(node->child, depth + 1);
//     }
    
//     if (node->next) {
//         print_ast(node->next, depth);
//     }
// }



#include "optimizer.hpp"
#include "stats.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Flag to enable/disable optimizations
int enable_selection_pushdown = 1;
int enable_projection_pushdown = 1;
int enable_join_reordering = 0;
int debugkaru = 0;

Node* duplicate_node(Node *node) {
    if (!node) return NULL;
    Node *new_ = new_node(node->operation, node->arg1, node->arg2);
    new_->child = duplicate_node(node->child);
    new_->next = duplicate_node(node->next);
    return new_;
}

void free_node(Node *node) {
    if (!node) return;
    free_node(node->child);
    free_node(node->next);
    if (node->operation) free(node->operation);
    if (node->arg1) free(node->arg1);
    if (node->arg2) free(node->arg2);
    free(node);
}

void extract_table_column(const char *expr, char **table, char **column) {
    char *expr_copy = strdup(expr);
    char *start = expr_copy;
    while (isspace(*start)) start++;
    char *end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) *end-- = '\0';
    
    char *dot = strchr(start, '.');
    if (dot) {
        int table_len = dot - start;
        *table = (char*) malloc(table_len + 1);
        strncpy(*table, start, table_len);
        (*table)[table_len] = '\0';
        *column = strdup(dot + 1);
    } else {
        *table = NULL;
        *column = strdup(start);
    }
    free(expr_copy);
}
static int count_columns(const char *columns) {
    if (!columns) return 0;
    int count = 0;
    char *copy = strdup(columns);
    char *token = strtok(copy, ",");
    while (token) {
        while (*token == ' ') token++;
        count++;
        token = strtok(NULL, ",");
    }
    free(copy);
    return count;
}

static int count_required_columns(Node *node) {
    // Walk up the tree to find what columns are needed
    Node *current = node;
    while (current) {
        if (current->operation && strcmp(current->operation, "π") == 0) {
            return count_columns(current->arg1);
        }
        current = current->next;
    }
    
    // If no projection found above, check children
    if (node->child) {
        CostMetrics child = estimate_cost(node->child);
        return child.num_columns;
    }
    
    return 4; // Default column count
}

void extract_condition_components(const char *condition, char **table, char **column, char **op, int *value) {
    char condition_copy[100];
    strcpy(condition_copy, condition);
    
    char *token = strtok(condition_copy, " ");
    if (token) {
        extract_table_column(token, table, column);
        token = strtok(NULL, " ");
        if (token) {
            *op = strdup(token);
            token = strtok(NULL, " ");
            if (token) {
                *value = atoi(token);
            } else {
                *value = 0;
            }
        } else {
            *op = NULL;
            *value = 0;
        }
    } else {
        *table = *column = *op = NULL;
        *value = 0;
    }
}

int can_push_to_table(const char *condition, const char *table_name, Node *context) {
    char *table = NULL, *column = NULL, *op = NULL;
    int value = 0;
    extract_condition_components(condition, &table, &column, &op, &value);

    int result = 0;
    if (table && column) {
        result = strcmp(table, table_name) == 0 && get_column_stats(table_name, column) != NULL;
    } else if (column) {
        ColumnStats *stats = get_column_stats(table_name, column);
        if (stats) result = 1;
    }
    
    if (table) free(table);
    if (column) free(column);
    if (op) free(op);
    return result;
}

void parse_join_condition(const char *condition, char **left_table, char **left_col, 
                         char **right_table, char **right_col) {
    char condition_copy[100];
    strcpy(condition_copy, condition);
    
    char *left_side = strtok(condition_copy, "=");
    char *right_side = strtok(NULL, "=");
    
    if (left_side && right_side) {
        char *left_clean = left_side;
        char *right_clean = right_side;
        while (isspace(*left_clean)) left_clean++;
        while (isspace(*right_clean)) right_clean++;
        char *left_end = left_clean + strlen(left_clean) - 1;
        char *right_end = right_clean + strlen(right_clean) - 1;
        while (left_end > left_clean && isspace(*left_end)) *left_end-- = '\0';
        while (right_end > right_clean && isspace(*right_end)) *right_end-- = '\0';
        
        extract_table_column(left_clean, left_table, left_col);
        extract_table_column(right_clean, right_table, right_col);
    } else {
        *left_table = *left_col = *right_table = *right_col = NULL;
    }
}

static double get_condition_selectivity(Node *node) {
    if (!node || !node->arg1) return 0.1;
    
    char *table = NULL, *column = NULL, *op = NULL;
    int value = 0;
    extract_condition_components(node->arg1, &table, &column, &op, &value);
    
    double selectivity = 0.1;
    if (table && column && op) {
        selectivity = calculate_condition_selectivity(table, column, op, value);
    } else if (column && op) {
        Node *current = node->child;
        while (current) {
            if (current->operation && strcmp(current->operation, "table") == 0) {
                if (get_column_stats(current->arg1, column)) {
                    selectivity = calculate_condition_selectivity(current->arg1, column, op, value);
                    break;
                }
            }
            current = current->child ? current->child : current->next;
        }
    }
    
    if (table) free(table);
    if (column) free(column);
    if (op) free(op);
    
    return selectivity;
}

static double get_join_selectivity(Node *node) {
    if (!node || !node->arg1) return 0.0001;
    
    char *left_table = NULL, *left_col = NULL;
    char *right_table = NULL, *right_col = NULL;
    parse_join_condition(node->arg1, &left_table, &left_col, &right_table, &right_col);
    
    double selectivity = 0.0001;
    if (left_table && right_table && left_col && right_col) {
        selectivity = calculate_join_selectivity(left_table, left_col, right_table, right_col);
    }
    
    if (left_table) free(left_table);
    if (left_col) free(left_col);
    if (right_table) free(right_table);
    if (right_col) free(right_col);
    
    return selectivity;
}

// static int is_table_scan(Node *node) {
//     return node && node->operation && strcmp(node->operation, "table") == 0;
// }


// static int count_columns(const char *columns) {
//     if (!columns) return 0;
//     int count = 0;
//     char *copy = strdup(columns);
//     char *token = strtok(copy, ",");
//     while (token) {
//         while (*token == ' ') token++;
//         count++;
//         token = strtok(NULL, ",");
//     }
//     free(copy);
//     return count;
// }

// static int get_table_columns(const char *table_name) {
//     TableStats *stats = get_table_stats(table_name);
//     return stats ? stats->column_count : 4; // Default to 4 if no stats
// }

// CostMetrics estimate_cost(Node *node) {
//     CostMetrics metrics = {0, 0, 0.0};
//     if (!node) return metrics;

//     if (strcmp(node->operation, "table") == 0) {
//         TableStats *stats = get_table_stats(node->arg1);
//         metrics.result_size = stats ? stats->row_count : 1000;
//         metrics.num_columns = stats ? stats->column_count : 4;
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "σ") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         double selectivity = get_condition_selectivity(node);
        
//         metrics.result_size = (int)(child.result_size * selectivity);
//         metrics.num_columns = child.num_columns; // Columns unchanged
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "⨝") == 0) {
//         if (!node->child || !node->child->next) return metrics;
        
//         CostMetrics left = estimate_cost(node->child);
//         CostMetrics right = estimate_cost(node->child->next);
//         double selectivity = get_join_selectivity(node);
        
//         metrics.result_size = (int)(left.result_size * right.result_size * selectivity);
//         metrics.num_columns = left.num_columns + right.num_columns; // Combine columns
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "π") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         metrics.result_size = child.result_size;
//         metrics.num_columns = count_columns(node->arg1); // Only selected columns
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     return metrics;
// }


// CostMetrics estimate_cost(Node *node) {
//     CostMetrics metrics = {0, 0, 0.0};
//     if (!node) return metrics;

//     if (strcmp(node->operation, "table") == 0) {
//         TableStats *stats = get_table_stats(node->arg1);
//         metrics.result_size = stats ? stats->row_count : 1000;
//         metrics.num_columns = stats ? stats->column_count : 4; // Full table scan
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }
//     else if (strcmp(node->operation, "π") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         metrics.result_size = child.result_size;
//         metrics.num_columns = count_columns(node->arg1); // Only projected columns
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "⨝") == 0) {
//         if (!node->child || !node->child->next) return metrics;
        
//         CostMetrics left = estimate_cost(node->child);
//         CostMetrics right = estimate_cost(node->child->next);
//         double selectivity = get_join_selectivity(node);
        
//         metrics.result_size = (int)(left.result_size * right.result_size * selectivity);
//         metrics.num_columns = left.num_columns + right.num_columns;
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "π") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         metrics.result_size = child.result_size;
//         metrics.num_columns = count_columns(node->arg1); // Only selected columns
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     return metrics;
// }

// CostMetrics estimate_cost(Node *node) {
//     CostMetrics metrics = {0, 0, 0.0};
//     if (!node) return metrics;

//     if (strcmp(node->operation, "table") == 0) {
//         TableStats *stats = get_table_stats(node->arg1);
//         metrics.result_size = stats ? stats->row_count : 1000;
//         metrics.num_columns = stats ? stats->column_count : 4;
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "σ") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         double selectivity = get_condition_selectivity(node);
        
//         // Ensure minimum 1 row if table has rows
//         metrics.result_size = child.result_size > 0 ? 
//                             (int)(child.result_size * selectivity) : 0;
//         if (metrics.result_size == 0 && child.result_size > 0) {
//             metrics.result_size = 1; // At least 1 row matches
//         }
//         metrics.num_columns = child.num_columns;
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "⨝") == 0) {
//         if (!node->child || !node->child->next) return metrics;
        
//         CostMetrics left = estimate_cost(node->child);
//         CostMetrics right = estimate_cost(node->child->next);
//         double selectivity = get_join_selectivity(node);
        
//         metrics.result_size = (int)(left.result_size * right.result_size * selectivity);
//         if (metrics.result_size == 0 && left.result_size > 0 && right.result_size > 0) {
//             metrics.result_size = 1; // At least 1 row matches
//         }
        
//         // Count only columns needed by parent operations
//         metrics.num_columns = count_required_columns(node);
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     if (strcmp(node->operation, "π") == 0) {
//         CostMetrics child = estimate_cost(node->child);
//         metrics.result_size = child.result_size;
//         metrics.num_columns = count_columns(node->arg1);
//         metrics.cost = metrics.result_size * metrics.num_columns;
//         return metrics;
//     }

//     return metrics;
// }

double calculate_total_plan_cost(Node *node) {
    if (!node) return 0.0;
    
    CostMetrics current = estimate_cost(node);
    double child_cost = calculate_total_plan_cost(node->child);
    double next_cost = calculate_total_plan_cost(node->next);
    
    return current.cost + child_cost + next_cost;
}
CostMetrics estimate_cost(Node *node) {
    CostMetrics metrics = {0, 0, 0.0};
    if (!node) return metrics;

    if (strcmp(node->operation, "table") == 0) {
        TableStats *stats = get_table_stats(node->arg1);
        metrics.result_size = stats ? stats->row_count : 1000;
        metrics.num_columns = stats ? stats->column_count : 4;
        metrics.cost = metrics.result_size * metrics.num_columns;
        return metrics;
    }

    if (strcmp(node->operation, "σ") == 0) {
        CostMetrics child = estimate_cost(node->child);
        double selectivity = get_condition_selectivity(node);
        
        metrics.result_size = (int)(child.result_size * selectivity);
        if (metrics.result_size == 0 && child.result_size > 0) {
            metrics.result_size = 1;
        }
        metrics.num_columns = child.num_columns;
        // Only count this node's cost, not child's
        metrics.cost = metrics.result_size * metrics.num_columns;
        return metrics;
    }

    if (strcmp(node->operation, "⨝") == 0) {
        if (!node->child || !node->child->next) return metrics;
        
        CostMetrics left = estimate_cost(node->child);
        CostMetrics right = estimate_cost(node->child->next);
        double selectivity = get_join_selectivity(node);
        
        metrics.result_size = (int)(left.result_size * right.result_size * selectivity);
        if (metrics.result_size == 0 && left.result_size > 0 && right.result_size > 0) {
            metrics.result_size = 1;
        }
        
        metrics.num_columns = count_required_columns(node);
        // Only count this node's cost
        metrics.cost = metrics.result_size * metrics.num_columns;
        return metrics;
    }

    if (strcmp(node->operation, "π") == 0) {
        CostMetrics child = estimate_cost(node->child);
        metrics.result_size = child.result_size;
        metrics.num_columns = count_columns(node->arg1);
        // Only count this node's cost
        metrics.cost = metrics.result_size * metrics.num_columns;
        return metrics;
    }

    return metrics;
}

Node* push_down_selections(Node *node) {
    if (!node) return NULL;
    if (debugkaru) printf("Pushing down selections...%s\n", node->operation ? node->operation : "NULL");
    
    if (node->operation && strcmp(node->operation, "σ") == 0) {
        Node *child = node->child;
        if (!child) return node;
        
        if (child->operation && strcmp(child->operation, "table") == 0 && 
            child->next && child->next->operation && strcmp(child->next->operation, "⨝") == 0) {
            
            Node *left_table = child;
            Node *join_node = child->next;
            Node *right_table = NULL;
            
            if (left_table->child && left_table->child->operation && 
                strcmp(left_table->child->operation, "table") == 0) {
                right_table = left_table->child;
            }
            
            if (right_table && can_push_to_table(node->arg1, right_table->arg1, node)) {
                if (debugkaru) printf("Pushing condition '%s' down to table '%s'\n", node->arg1, right_table->arg1);
                
                Node *new_selection = new_node("σ", strdup(node->arg1), NULL);
                new_selection->child = right_table;
                
                left_table->child = NULL;
                
                join_node->child = left_table;
                left_table->next = new_selection;
                
                node->child = NULL;
                free(node->operation);
                free(node->arg1);
                free(node);
                
                return join_node;
            } 
            else if (can_push_to_table(node->arg1, left_table->arg1, node)) {
                if (debugkaru) printf("Pushing condition '%s' down to table '%s'\n", node->arg1, left_table->arg1);
                
                Node *new_selection = new_node("σ", strdup(node->arg1), NULL);
                new_selection->child = left_table;
                
                left_table->child = NULL;
                
                join_node->child = new_selection;
                new_selection->next = right_table;
                
                node->child = NULL;
                free(node->operation);
                free(node->arg1);
                free(node);
                
                return join_node;
            }
        }
    }
    
    if (node->child) node->child = push_down_selections(node->child);
    if (node->next) node->next = push_down_selections(node->next);
    
    return node;
}

// Helper function to check if a column is in the projection list
static int is_column_in_projection(const char *column, const char *projection_list) {
    if (!column || !projection_list) return 0;
    
    char *copy = strdup(projection_list);
    char *token = strtok(copy, ",");
    while (token) {
        // Trim whitespace
        while (*token == ' ') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace(*end)) *end-- = '\0';
        
        if (strcmp(token, column) == 0) {
            free(copy);
            return 1;
        }
        token = strtok(NULL, ",");
    }
    free(copy);
    return 0;
}

// Function to create a restricted projection node
static Node* create_restricted_projection(Node *node, const char *projection_list) {
    if (!node || !projection_list) return node;
    
    // For table nodes, create a projection with only the needed columns
    if (strcmp(node->operation, "table") == 0) {
        TableStats *stats = get_table_stats(node->arg1);
        if (!stats) return node;
        
        char restricted_columns[1024] = "";
        int first = 1;
        
        // Check each column in the table
        for (int i = 0; i < stats->column_count; i++) {
            char full_col[256];
            sprintf(full_col, "%s.%s", node->arg1, stats->column_names[i]);
            
            if (is_column_in_projection(full_col, projection_list) ||
                is_column_in_projection(stats->column_names[i], projection_list)) {
                if (!first) strcat(restricted_columns, ",");
                strcat(restricted_columns, full_col);
                first = 0;
            }
        }
        
        // If we found columns to keep, create a projection
        if (strlen(restricted_columns) > 0) {
            Node *proj = new_node("π", strdup(restricted_columns), NULL);
            proj->child = node;
            return proj;
        }
    }
    
    return node;
}
// Node* push_down_projections(Node *node) {
//     if (!node) {
//         if (debugkaru) printf("\n[DEBUG] Node is NULL, returning NULL\n");
//         return NULL;
//     }
    
//     if (debugkaru) printf("\n[DEBUG] ENTER push_down_projections for node type: %s, arg1: %s\n", 
//            node->operation ? node->operation : "NULL", 
//            node->arg1 ? node->arg1 : "NULL");
    
//     if (node->operation && strcmp(node->operation, "π") == 0) {
//         if (debugkaru) printf("\n[DEBUG] Found projection node with columns: %s\n", node->arg1);
        
//         Node *child = node->child;
//         if (!child) {
//             if (debugkaru) printf("\n[DEBUG] Projection has no child, returning node\n");
//             return node;
//         }
        
//         if (debugkaru) printf("\n[DEBUG] Child operation: %s\n", child->operation ? child->operation : "NULL");
        
//         if (child->operation && strcmp(child->operation, "σ") == 0) {
//             if (debugkaru) printf("\n[DEBUG] Case 1: Push projection through selection\n");
            
//             if (debugkaru) printf("\n[DEBUG] Selection condition: %s\n", child->arg1 ? child->arg1 : "NULL");
            
//             Node *new_projection = new_node("π", strdup(node->arg1), NULL);
//             if (debugkaru) printf("\n[DEBUG] Created new_ projection with columns: %s\n", new_projection->arg1);
            
//             new_projection->child = child->child;
//             if (debugkaru) printf("\n[DEBUG] Connected new_ projection to selection's child\n");
            
//             child->child = new_projection;
//             if (debugkaru) printf("\n[DEBUG] Put new_ projection under selection\n");
            
//             if (debugkaru) printf("\n[DEBUG] Returning selection node as new_ root\n");
//             Node *result = child;
//             node->child = NULL;
//             free(node->operation);
//             free(node->arg1);
//             free(node);
            
//             if (debugkaru) printf("\n[DEBUG] Recursively optimizing projection under selection\n");
//             result->child = push_down_projections(result->child);
            
//             if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with selection root\n");
//             return result;
//         }
//         else if (child->operation && strcmp(child->operation, "table") == 0 && 
//                  child->next && child->next->operation && strcmp(child->next->operation, "⨝") == 0) {
            
//             if (debugkaru) printf("\n[DEBUG] Case 2: Push projection through join\n");
            
//             Node *left_table = child;
//             Node *join_node = child->next;
            
//             if (debugkaru) printf("\n[DEBUG] Left table: %s\n", left_table->arg1);
//             if (debugkaru) printf("\n[DEBUG] Join condition: %s\n", join_node->arg1);
            
//             Node *right_table = NULL;
//             if (left_table->child && left_table->child->operation && 
//                 strcmp(left_table->child->operation, "table") == 0) {
//                 right_table = left_table->child;
//                 if (debugkaru) printf("\n[DEBUG] Found right table as child of left table: %s\n", right_table->arg1);
//             } else {
//                 if (debugkaru) printf("\n[DEBUG] Could not find right table, returning original node\n");
//                 if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with original node\n");
//                 return node;
//             }
            
//             if (debugkaru) printf("\n[DEBUG] Parsing projection columns: %s\n", node->arg1);
//             char *columns = strdup(node->arg1);
//             char *column_list[100];
//             int column_count = 0;
            
//             char *token = strtok(columns, ",");
//             while (token) {
//                 while (*token == ' ') token++;
//                 column_list[column_count] = strdup(token);
//                 if (debugkaru) printf("\n[DEBUG] Column %d: %s\n", column_count, column_list[column_count]);
//                 column_count++;
//                 token = strtok(NULL, ",");
//             }
            
//             if (debugkaru) printf("\n[DEBUG] Parsing join condition: %s\n", join_node->arg1);
//             char *left_table_name = NULL, *left_col = NULL;
//             char *right_table_name = NULL, *right_col = NULL;
//             parse_join_condition(join_node->arg1, &left_table_name, &left_col, 
//                                &right_table_name, &right_col);
            
//             if (debugkaru) printf("\n[DEBUG] Join parsed: %s.%s = %s.%s\n", 
//                    left_table_name ? left_table_name : "NULL", 
//                    left_col ? left_col : "NULL", 
//                    right_table_name ? right_table_name : "NULL", 
//                    right_col ? right_col : "NULL");
            
//             char left_columns[512] = "";
//             char right_columns[512] = "";
//             int left_has_columns = 0;
//             int right_has_columns = 0;
            
//             if (debugkaru) printf("\n[DEBUG] Sorting columns by table\n");
//             for (int i = 0; i < column_count; i++) {
//                 char *table = NULL, *column = NULL;
//                 extract_table_column(column_list[i], &table, &column);
                
//                 if (debugkaru) printf("\n[DEBUG] Column %d: table=%s, column=%s\n", 
//                        i, table ? table : "NULL", column ? column : "NULL");
                
//                 if (table) {
//                     if (strcmp(table, left_table->arg1) == 0) {
//                         if (left_has_columns) strcat(left_columns, ",");
//                         strcat(left_columns, column_list[i]);
//                         left_has_columns = 1;
//                         if (debugkaru) printf("\n[DEBUG] Added to left columns: %s\n", column_list[i]);
//                     }
//                     else if (strcmp(table, right_table->arg1) == 0) {
//                         if (right_has_columns) strcat(right_columns, ",");
//                         strcat(right_columns, column_list[i]);
//                         right_has_columns = 1;
//                         if (debugkaru) printf("\n[DEBUG] Added to right columns: %s\n", column_list[i]);
//                     }
//                 }
                
//                 if (table) free(table);
//                 if (column) free(column);
//             }
            
//             if (left_table_name && left_col) {
//                 char full_col[100];
//                 sprintf(full_col, "%s.%s", left_table_name, left_col);
//                 if (!strstr(left_columns, full_col)) {
//                     if (left_has_columns) strcat(left_columns, ",");
//                     strcat(left_columns, full_col);
//                     left_has_columns = 1;
//                     if (debugkaru) printf("\n[DEBUG] Added join column to left: %s\n", full_col);
//                 }
//             }
            
//             if (right_table_name && right_col) {
//                 char full_col[100];
//                 sprintf(full_col, "%s.%s", right_table_name, right_col);
//                 if (!strstr(right_columns, full_col)) {
//                     if (right_has_columns) strcat(right_columns, ",");
//                     strcat(right_columns, full_col);
//                     right_has_columns = 1;
//                     if (debugkaru) printf("\n[DEBUG] Added join column to right: %s\n", full_col);
//                 }
//             }
            
//             if (debugkaru) printf("\n[DEBUG] Final left columns: %s\n", left_columns);
//             if (debugkaru) printf("\n[DEBUG] Final right columns: %s\n", right_columns);
            
//             Node *new_left_table = new_node("table", strdup(left_table->arg1), NULL);
//             Node *new_right_table = new_node("table", strdup(right_table->arg1), NULL);
            
//             Node *left_projection = NULL;
//             if (left_has_columns) {
//                 left_projection = new_node("π", strdup(left_columns), NULL);
//                 left_projection->child = new_left_table;
//                 if (debugkaru) printf("\n[DEBUG] Created left projection: π(%s)\n", left_columns);
//             } else {
//                 left_projection = new_left_table;
//                 if (debugkaru) printf("\n[DEBUG] No left projection needed\n");
//             }
            
//             Node *right_projection = NULL;
//             if (right_has_columns) {
//                 right_projection = new_node("π", strdup(right_columns), NULL);
//                 right_projection->child = new_right_table;
//                 if (debugkaru) printf("\n[DEBUG] Created right projection: π(%s)\n", right_columns);
//             } else {
//                 right_projection = new_right_table;
//                 if (debugkaru) printf("\n[DEBUG] No right projection needed\n");
//             }
            
//             Node *new_join = new_node("⨝", strdup(join_node->arg1), NULL);
            
//             if (debugkaru) printf("\n[DEBUG] Rebuilding tree with clean structure\n");
            
//             new_join->child = left_projection;
//             left_projection->next = right_projection;
            
//             Node *top_projection = new_node("π", strdup(node->arg1), NULL);
//             top_projection->child = new_join;
            
//             for (int i = 0; i < column_count; i++) {
//                 free(column_list[i]);
//             }
//             free(columns);
//             if (left_table_name) free(left_table_name);
//             if (left_col) free(left_col); 
//             if (right_table_name) free(right_table_name);
//             if (right_col) free(right_col);
            
//             node->child = NULL;
//             free(node->operation);
//             free(node->arg1);
//             free(node);
            
//             if (debugkaru) printf("\n[DEBUG] Tree rebuilt with clean structure, returning new_ top projection\n");
//             if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with rebuilt tree\n");
//             return top_projection;
//         }
//     }
    
//     if (node->child) {
//         if (debugkaru) printf("\n[DEBUG] Recursively processing child of %s\n", node->operation);
//         node->child = push_down_projections(node->child);
//     }
//     if (node->next) {
//         if (debugkaru) printf("\n[DEBUG] Recursively processing next of %s\n", node->operation);
//         node->next = push_down_projections(node->next);
//     }
    
//     if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections returning node type: %s\n", 
//            node->operation ? node->operation : "NULL");
//     return node;
// }

// Node* push_down_projections(Node *node) {
//     if (!node) return NULL;

//     if (node->operation && strcmp(node->operation, "π") == 0) {
//         // Get the projection columns
//         char *projection_list = node->arg1;
        
//         // Push down to child nodes
//         if (node->child) {
//             node->child = push_down_projections(node->child);
            
//             // Add projections to restrict columns in child nodes
//             if (strcmp(node->child->operation, "⨝") == 0) {
//                 // Handle join - push to both sides
//                 if (node->child->child) {
//                     node->child->child = create_restricted_projection(node->child->child, projection_list);
//                 }
//                 if (node->child->next) {
//                     node->child->next = create_restricted_projection(node->child->next, projection_list);
//                 }
//             }
//             else {
//                 // For other operations, just restrict the child
//                 node->child = create_restricted_projection(node->child, projection_list);
//             }
//         }
        
//         return node;
//     }

//     // Recursively process children
//     if (node->child) node->child = push_down_projections(node->child);
//     if (node->next) node->next = push_down_projections(node->next);
    
//     return node;
// }



Node* push_down_projections(Node *node) {
    if (!node) {
        if (debugkaru) printf("\n[DEBUG] Node is NULL, returning NULL\n");
        return NULL;
    }
    
    if (debugkaru) printf("\n[DEBUG] ENTER push_down_projections for node type: %s, arg1: %s\n", 
           node->operation ? node->operation : "NULL", 
           node->arg1 ? node->arg1 : "NULL");
    
    if (node->operation && strcmp(node->operation, "π") == 0) {
        if (debugkaru) printf("\n[DEBUG] Found projection node with columns: %s\n", node->arg1);
        
        Node *child = node->child;
        if (!child) {
            if (debugkaru) printf("\n[DEBUG] Projection has no child, returning node\n");
            return node;
        }
        
        if (debugkaru) printf("\n[DEBUG] Child operation: %s\n", child->operation ? child->operation : "NULL");
        
        if (child->operation && strcmp(child->operation, "σ") == 0) {
            if (debugkaru) printf("\n[DEBUG] Case 1: Push projection through selection\n");
            
            if (debugkaru) printf("\n[DEBUG] Selection condition: %s\n", child->arg1 ? child->arg1 : "NULL");
            
            Node *new_projection = new_node("π", strdup(node->arg1), NULL);
            if (debugkaru) printf("\n[DEBUG] Created new projection with columns: %s\n", new_projection->arg1);
            
            new_projection->child = child->child;
            if (debugkaru) printf("\n[DEBUG] Connected new projection to selection's child\n");
            
            child->child = new_projection;
            if (debugkaru) printf("\n[DEBUG] Put new projection under selection\n");
            
            if (debugkaru) printf("\n[DEBUG] Returning selection node as new root\n");
            Node *result = child;
            node->child = NULL;
            free(node->operation);
            free(node->arg1);
            free(node);
            
            if (debugkaru) printf("\n[DEBUG] Recursively optimizing projection under selection\n");
            result->child = push_down_projections(result->child);
            
            if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with selection root\n");
            return result;
        }
        else if (child->operation && strcmp(child->operation, "table") == 0 && 
                 child->next && child->next->operation && strcmp(child->next->operation, "⨝") == 0) {
            
            if (debugkaru) printf("\n[DEBUG] Case 2: Push projection through join\n");
            
            Node *left_table = child;
            Node *join_node = child->next;
            
            if (debugkaru) printf("\n[DEBUG] Left table: %s\n", left_table->arg1);
            if (debugkaru) printf("\n[DEBUG] Join condition: %s\n", join_node->arg1);
            
            Node *right_table = NULL;
            if (left_table->child && left_table->child->operation && 
                strcmp(left_table->child->operation, "table") == 0) {
                right_table = left_table->child;
                if (debugkaru) printf("\n[DEBUG] Found right table as child of left table: %s\n", right_table->arg1);
            } else {
                if (debugkaru) printf("\n[DEBUG] Could not find right table, returning original node\n");
                if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with original node\n");
                return node;
            }
            
            if (debugkaru) printf("\n[DEBUG] Parsing projection columns: %s\n", node->arg1);
            char *columns = strdup(node->arg1);
            char *column_list[100];
            int column_count = 0;
            
            // char *token = strtok(columns, ",");
            // while (token) {
            //     while (*token == ' ') token++;
            //     column_list[column_count] = strdup(token);
            //     if (debugkaru) printf("\n[DEBUG] Column %d: %s\n", column_count, column_list[column_count]);
            //     column_count++;
            //     token = strtok(NULL, ",");
            // }
            // In push_down_projections(), modify the column parsing section:
char *token = strtok(columns, ",");
while (token) {
    // Trim whitespace and remove empty entries
    while (*token == ' ') token++;
    char *end = token + strlen(token) - 1;
    while (end > token && isspace(*end)) *end-- = '\0';
    
    if (*token != '\0') {  // Skip empty entries
        column_list[column_count] = strdup(token);
        if (debugkaru) printf("\n[DEBUG] Column %d: %s\n", column_count, column_list[column_count]);
        column_count++;
    }
    token = strtok(NULL, ",");
}

            if (debugkaru) printf("\n[DEBUG] Parsing join condition: %s\n", join_node->arg1);
            char *left_table_name = NULL, *left_col = NULL;
            char *right_table_name = NULL, *right_col = NULL;
            parse_join_condition(join_node->arg1, &left_table_name, &left_col, 
                               &right_table_name, &right_col);
            
            if (debugkaru) printf("\n[DEBUG] Join parsed: %s.%s = %s.%s\n", 
                   left_table_name ? left_table_name : "NULL", 
                   left_col ? left_col : "NULL", 
                   right_table_name ? right_table_name : "NULL", 
                   right_col ? right_col : "NULL");
            
            char left_columns[512] = "";
            char right_columns[512] = "";
            int left_has_columns = 0;
            int right_has_columns = 0;
            
            // NEW: Track required columns for join condition
            char required_left_columns[512] = "";
            char required_right_columns[512] = "";
            if (left_table_name && left_col) {
                sprintf(required_left_columns, "%s.%s", left_table_name, left_col);
                left_has_columns = 1;
            }
            if (right_table_name && right_col) {
                sprintf(required_right_columns, "%s.%s", right_table_name, right_col);
                right_has_columns = 1;
            }
            
            if (debugkaru) printf("\n[DEBUG] Sorting columns by table\n");
            for (int i = 0; i < column_count; i++) {
                char *table = NULL, *column = NULL;
                extract_table_column(column_list[i], &table, &column);
                
                if (debugkaru) printf("\n[DEBUG] Column %d: table=%s, column=%s\n", 
                       i, table ? table : "NULL", column ? column : "NULL");
                
                if (table) {
                    if (strcmp(table, left_table->arg1) == 0) {
                        if (left_has_columns) strcat(left_columns, ",");
                        strcat(left_columns, column_list[i]);
                        left_has_columns = 1;
                        if (debugkaru) printf("\n[DEBUG] Added to left columns: %s\n", column_list[i]);
                    }
                    else if (strcmp(table, right_table->arg1) == 0) {
                        if (right_has_columns) strcat(right_columns, ",");
                        strcat(right_columns, column_list[i]);
                        right_has_columns = 1;
                        if (debugkaru) printf("\n[DEBUG] Added to right columns: %s\n", column_list[i]);
                    }
                }
                
                if (table) free(table);
                if (column) free(column);
            }
            
            // NEW: Add join columns if not already included
            if (strlen(required_left_columns) > 0 && !strstr(left_columns, required_left_columns)) {
                if (left_has_columns) strcat(left_columns, ",");
                strcat(left_columns, required_left_columns);
                if (debugkaru) printf("\n[DEBUG] Added join column to left: %s\n", required_left_columns);
            }
            
            if (strlen(required_right_columns) > 0 && !strstr(right_columns, required_right_columns)) {
                if (right_has_columns) strcat(right_columns, ",");
                strcat(right_columns, required_right_columns);
                if (debugkaru) printf("\n[DEBUG] Added join column to right: %s\n", required_right_columns);
            }
            
            if (debugkaru) printf("\n[DEBUG] Final left columns: %s\n", left_columns);
            if (debugkaru) printf("\n[DEBUG] Final right columns: %s\n", right_columns);
            
            Node *new_left_table = new_node("table", strdup(left_table->arg1), NULL);
            Node *new_right_table = new_node("table", strdup(right_table->arg1), NULL);
            
            Node *left_projection = NULL;
            if (left_has_columns || strlen(required_left_columns) > 0) {
                left_projection = new_node("π", strdup(left_columns), NULL);
                left_projection->child = new_left_table;
                if (debugkaru) printf("\n[DEBUG] Created left projection: π(%s)\n", left_columns);
            } else {
                left_projection = new_left_table;
                if (debugkaru) printf("\n[DEBUG] No left projection needed\n");
            }
            
            Node *right_projection = NULL;
            if (right_has_columns || strlen(required_right_columns) > 0) {
                right_projection = new_node("π", strdup(right_columns), NULL);
                right_projection->child = new_right_table;
                if (debugkaru) printf("\n[DEBUG] Created right projection: π(%s)\n", right_columns);
            } else {
                right_projection = new_right_table;
                if (debugkaru) printf("\n[DEBUG] No right projection needed\n");
            }
            
            Node *new_join = new_node("⨝", strdup(join_node->arg1), NULL);
            
            if (debugkaru) printf("\n[DEBUG] Rebuilding tree with clean structure\n");
            
            new_join->child = left_projection;
            left_projection->next = right_projection;
            
            Node *top_projection = new_node("π", strdup(node->arg1), NULL);
            top_projection->child = new_join;
            
            for (int i = 0; i < column_count; i++) {
                free(column_list[i]);
            }
            free(columns);
            if (left_table_name) free(left_table_name);
            if (left_col) free(left_col); 
            if (right_table_name) free(right_table_name);
            if (right_col) free(right_col);
            
            node->child = NULL;
            free(node->operation);
            free(node->arg1);
            free(node);
            
            if (debugkaru) printf("\n[DEBUG] Tree rebuilt with clean structure, returning new top projection\n");
            if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections with rebuilt tree\n");
            return top_projection;
        }
    }
    
    if (node->child) {
        if (debugkaru) printf("\n[DEBUG] Recursively processing child of %s\n", node->operation);
        node->child = push_down_projections(node->child);
    }
    if (node->next) {
        if (debugkaru) printf("\n[DEBUG] Recursively processing next of %s\n", node->operation);
        node->next = push_down_projections(node->next);
    }
    
    if (debugkaru) printf("\n[DEBUG] EXIT push_down_projections returning node type: %s\n", 
           node->operation ? node->operation : "NULL");
    return node;
}
// void print_execution_plan_recursive(Node *node, int depth) {
//     if (!node) return;
    
//     for (int i = 0; i < depth; i++) printf("  ");
    
//     CostMetrics metrics = estimate_cost(node);
    
//     if (strcmp(node->operation, "table") == 0) {
//         printf("table(%s) [size=%d, cols=%d, cost=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
//     } else if (strcmp(node->operation, "σ") == 0) {
//         printf("σ(%s) [size=%d, cols=%d, cost=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
//     } else if (strcmp(node->operation, "⨝") == 0) {
//         printf("⨝(%s) [size=%d, cols=%d, cost=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
//     } else if (strcmp(node->operation, "π") == 0) {
//         printf("π(%s) [size=%d, cols=%d, cost=%.1f]\n", 
//                node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
//     } else {
//         printf("%s %s %s [size=%d, cols=%d, cost=%.1f]\n", 
//                node->operation, 
//                node->arg1 ? node->arg1 : "", 
//                node->arg2 ? node->arg2 : "", 
//                metrics.result_size, metrics.num_columns, metrics.cost);
//     }
    
//     print_execution_plan_recursive(node->child, depth + 1);
//     print_execution_plan_recursive(node->next, depth);
// }

// static int count_columns(const char *columns) {
//     if (!columns) return 0;
//     int count = 0;
//     char *copy = strdup(columns);
//     char *token = strtok(copy, ",");
//     while (token) {
//         while (*token == ' ') token++;
//         count++;
//         token = strtok(NULL, ",");
//     }
//     free(copy);
//     return count;
// }



static int get_table_columns(const char *table_name) {
    TableStats *stats = get_table_stats(table_name);
    return stats ? stats->column_count : 4; // Default to 4 if no stats
}

void print_execution_plan_recursive(Node *node, int depth) {
    if (!node) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    
    CostMetrics metrics = estimate_cost(node);
    
    if (strcmp(node->operation, "table") == 0) {
        printf("table(%s) [rows=%d, cols=%d, cost=%.1f]\n", 
               node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
    }
    else if (strcmp(node->operation, "σ") == 0) {
        printf("σ(%s) [rows=%d, cols=%d, cost=%.1f]\n", 
               node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
    }
    else if (strcmp(node->operation, "⨝") == 0) {
        printf("⨝(%s) [rows=%d, cols=%d, cost=%.1f]\n", 
               node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
    }
    else if (strcmp(node->operation, "π") == 0) {
        printf("π(%s) [rows=%d, cols=%d, cost=%.1f]\n", 
               node->arg1, metrics.result_size, metrics.num_columns, metrics.cost);
    }
    else {
        printf("%s %s %s [rows=%d, cols=%d, cost=%.1f]\n", 
               node->operation, 
               node->arg1 ? node->arg1 : "", 
               node->arg2 ? node->arg2 : "", 
               metrics.result_size, metrics.num_columns, metrics.cost);
    }
    
    print_execution_plan_recursive(node->child, depth + 1);
    print_execution_plan_recursive(node->next, depth);
}

void print_execution_plan(Node *node, const char *title) {
    printf("--- %s ---\n", title);
    print_execution_plan_recursive(node, 0);
}

Node* optimize_query(Node *root) {
    if (!root) return NULL;
    
    printf("\nOptimizing query...\n");
    
    init_stats();
    
    printf("\nOriginal Execution Plan:\n");
    CostMetrics original_cost = estimate_cost(root);
    print_execution_plan(root, "Original Plan");
    
    Node *original_root = duplicate_node(root);
    
    Node *selection_optimized = duplicate_node(original_root);
    CostMetrics selection_cost = {0, 0, 0.0};
    if (enable_selection_pushdown) {
        printf("\nApplying selection push-down...\n");
        selection_optimized = push_down_selections(selection_optimized);
        selection_cost = estimate_cost(selection_optimized);
        print_execution_plan(selection_optimized, "Selection Pushdown Plan");
    } else {
        selection_cost = original_cost;
    }
    
    Node *projection_optimized = duplicate_node(original_root);
    CostMetrics projection_cost = {0, 0, 0.0};
    if (enable_projection_pushdown) {
        printf("\nApplying projection push-down...\n");
        printf("I am ghere..\n");
        projection_optimized = push_down_projections(projection_optimized);
        projection_cost = estimate_cost(projection_optimized);
        print_execution_plan(projection_optimized, "Projection Pushdown Plan");
    } else {
        projection_cost = original_cost;
    }
    
// Replace the cost comparison with:
double original_total = calculate_total_plan_cost(root);
double selection_total = calculate_total_plan_cost(selection_optimized);
double projection_total = calculate_total_plan_cost(projection_optimized);
    
    Node *best_plan = root;
    CostMetrics best_cost = original_cost;
    const char *best_plan_name = "Original";
    double best_total = original_total;
    
    if (selection_total <= best_total) {
        best_plan = selection_optimized;
        best_cost = selection_cost;
        best_plan_name = "Selection Pushdown";
        best_total = selection_total;
    } else {
        free_node(selection_optimized);
    }
    
    if (projection_total <= best_total) {
        if (best_plan != root) free_node(best_plan);
        best_plan = projection_optimized;
        best_cost = projection_cost;
        best_plan_name = "Projection Pushdown";
        best_total = projection_total;
    } else {
        free_node(projection_optimized);
    }
    
    if (best_plan != root) {
        free_node(original_root);
    }
    
    printf("\nCost Comparison:\n");
    printf("Metric          | Original      | Selection     | Projection    | Best Plan\n");
    printf("----------------|---------------|---------------|---------------|----------\n");
    printf("Result Size     | %-13d | %-13d | %-13d | %s\n",
           original_cost.result_size, selection_cost.result_size, projection_cost.result_size,
           best_plan_name);
    printf("Columns         | %-13d | %-13d | %-13d | %s\n",
           original_cost.num_columns, selection_cost.num_columns, projection_cost.num_columns,
           best_plan_name);
    printf("Total Cost      | %-13.1f | %-13.1f | %-13.1f | %s\n",
           original_total, selection_total, projection_total, best_plan_name);
    
    printf("\nSelected Best Execution Plan (%s):\n", best_plan_name);
    print_execution_plan(best_plan, "Best Plan");
    
    return best_plan;
}

