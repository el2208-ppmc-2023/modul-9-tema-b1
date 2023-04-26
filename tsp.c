#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LENGTH 100
#define RADIUS 6371.0 // Radius of the Earth in kilometers

double to_radians(double degrees) {
    return degrees * 3.1415926 / 180.0;
}

double haversine(double x1, double y1, double x2, double y2) {
    double d_x = to_radians(x2 - x1);
    double d_y = to_radians(y2 - y1);
    double a = sin(d_x / 2) * sin(d_x / 2) +
               cos(to_radians(x1)) * cos(to_radians(x2)) *
               sin(d_y / 2) * sin(d_y / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return RADIUS * c;
}

typedef struct {
    char address[MAX_LENGTH];
    double x;
    double y;
} Point;

void nn_tsp(Point *points, int num_points) {
    // Create array to store visited status of each point
    int *visited = (int*)malloc(num_points*sizeof(int));
    for (int i = 0; i < num_points; i++) {
        visited[i] = 0;
    }
    
    // Start at first point
    int current_point = 0;
    visited[current_point] = 1;
    
    // Create array to store path
    int *path = (int*)malloc(num_points*sizeof(int));
    path[0] = current_point;
    
    // Visit all other points in nearest neighbor order
    for (int i = 1; i < num_points; i++) {
        int nearest_neighbor = -1;
        double nearest_neighbor_distance = -1;
        for (int j = 0; j < num_points; j++) {
            if (!visited[j]) {
                double d = haversine(points[current_point].x, points[current_point].y, points[j].x, points[j].y);
                if (nearest_neighbor == -1 || d < nearest_neighbor_distance) {
                    nearest_neighbor = j;
                    nearest_neighbor_distance = d;
                }
            }
        }
        current_point = nearest_neighbor;
        visited[current_point] = 1;
        path[i] = current_point;
    }
    
    // Print path and distance
    printf("Optimal path:\n");
    double total_distance = 0;
    for (int i = 0; i < num_points; i++) {
        printf("%s -> ", points[path[i]].address);
        if (i < num_points-1) {
            total_distance += haversine(points[path[i]].x, points[path[i]].y, points[path[i+1]].x, points[path[i+1]].y);
        }
        else{
            printf("%s", points[0]);
            total_distance += haversine(points[path[i]].x, points[path[i]].y, points[0].x, points[0].y);
            printf("\nTotal distance: %f\n", total_distance);
        }
    }
    
    // Free memory
    free(visited);
    free(path);
}

int main() {
    // Read data from file
    FILE *file;
    char line[MAX_LENGTH];
    Point *points = NULL;
    int num_points = 0;
    
    file = fopen("pelabuhan.txt", "r");
    if (file == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    // Read file line by line
    while (fgets(line, MAX_LENGTH, file) != NULL) {
        // Skip header
        if (strstr(line, "Pelabuhan") != NULL) {
            continue;
        }
        // Parse line and store data in points array
        char *token = strtok(line, ",");
        Point point;
        strcpy(point.address, token);
        token = strtok(NULL, ",");
        point.x = atof(token);
        token = strtok(NULL, ",");
        point.y = atof(token);
        points = (Point*)realloc(points, (num_points+1)*sizeof(Point));
        points[num_points] = point;
        num_points++;
    }
    
    fclose(file);

    // Solve TSP using nearest neighbor algorithm
    nn_tsp(points, num_points);

    // Free memory
    free(points);
    return 0;
}
