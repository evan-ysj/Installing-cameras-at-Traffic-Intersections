#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>


// Line-line intersection check; Return true if the two segments have intersection which is not end point
bool Cross(std::vector<int> p1, std::vector<int> p2, std::vector<int> p3, std::vector<int> p4) {
    float x1 = p1[0], x2 = p2[0], x3 = p3[0], x4 = p4[0];
    float y1 = p1[1], y2 = p2[1], y3 = p3[1], y4 = p4[1];  
    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if(denominator != 0) {
        float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
        float u = ((y1 - y2) * (x1 - x3) - (x1 - x2) * (y1 - y3)) / denominator;
        if(0 <= t && t <= 1 && 0 <= u && u <= 1) {
            float x = ((x1*y2 - x2*y1) * (x3 - x4) - (x1 - x2) * (x3*y4 - x4*y3)) / denominator;
            float y = ((x1*y2 - x2*y1) * (y3 - y4) - (y1 - y2) * (x3*y4 - x4*y3)) / denominator;
            if((x == x1 && y == y1) || (x == x2 && y == y2) || (x == x3 && y == y3) || (x == x4 && y == y4)) {
                return false;
            }
            else {
                return true;
            }    
        }
    }
    return false;
}

// Line-line overlap check; return true if two segments overlap
bool Overlap(std::vector<int> p1, std::vector<int> p2, std::vector<int> p3, std::vector<int> p4) {
    int x1 = p1[0], x2 = p2[0], x3 = p3[0], x4 = p4[0];
    int y1 = p1[1], y2 = p2[1], y3 = p3[1], y4 = p4[1];
    bool con1 = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4) == 0);
    bool con2 = ((x1 - x3) * (y2 - y4) - (y1 - y3) * (x2 - x4) == 0);
    bool con3 = ((x1 - x4) * (y2 - y3) - (y1 - y4) * (x2 - x3) == 0);
    bool con4 = (x1 <= x3 && x3 <= x2) || (x1 <= x4 && x4 <= x2) || (y1 <= y3 && y3 <= y2) || (y1 <= y4 && y4 <= y2);
    if(con1 && con2 && con3 && con4) {
        return true;
    }
    return false;
}

// Check the validation of a new street
bool CheckStreet(const std::vector<std::vector<std::vector<int>>> street_coord, const std::vector<std::vector<int>> last_street, int *intersect) {
    for(int i = 0; i < street_coord.size(); ++i) {
        for(int j = 1; j < last_street.size(); ++j) {
            for(int k = 1; k < street_coord[i].size(); ++k) {
                if(Cross(last_street[j-1], last_street[j], street_coord[i][k-1], street_coord[i][k])) {
                    *intersect = 1;
                }
                if(Overlap(last_street[j-1], last_street[j], street_coord[i][k-1], street_coord[i][k])) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Check the validation of a new segment
bool CheckSegment(const std::vector<std::vector<int>> coordinates, const std::vector<int> coord) {
    int size = coordinates.size();
    for(int i = 1; i < coordinates.size(); ++i) {  
        
        if(Overlap(coordinates[i-1], coordinates[i], coordinates[size-1], coord) || 
           Cross(coordinates[i-1], coordinates[i], coordinates[size-1], coord)) {
            return false;
        }
        if(coordinates[size-1][0] == coord[0] && coordinates[size-1][1] == coord[1]) {
            return false;
        }
    }
    return true;
}

// Print the output of rgen
void PrintCommand(const std::vector<char> street_names, const std::vector<std::vector<std::vector<int>>> street_coord) {
    for(int i = 0; i < street_coord.size(); ++i) {
        std::cout << "a \"" << street_names[i] << "\" ";
        for(int j = 0; j < street_coord[i].size(); ++j) {
            std::cout << "(" << street_coord[i][j][0] << "," << street_coord[i][j][1] << ")";
        }
        std::cout << "\n";
    }
    std::cout << "g" << "\n";
    std::cout.flush();
}

// Generate random street information, and store street names and segments in vectors
void PyGenerator(int s, int n, int l, int c) {
    // Check status of urandom
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return;
    }
    std::vector<std::vector<std::vector<int>>> street_coord;
    std::vector<char> street_names;
    int street = s, segment = n, delay = l, co_range = 2 * c;
    int count_seg = 30;
    int count_street = 25;
    int count_street_old = 0;
    while(true) {
        // Remove old data in a2
        if(!street_names.empty()) {
            for(char n: street_names) {
                std::cout << "r \"" << n << "\"\n";
            }
        }
        // Clear previous street name and segment information
        street_names.clear();
        street_coord.clear();
        // Generate the number of streets
        urandom.read((char*)&street, 1);
        street = 2 + street % (s - 1);   
        int *intersect = &count_seg;
        *intersect = 0;
        for(int i = 0; i < street; ++i) {
            // Define a vector to store coordinates of current street
            std::vector<std::vector<int>> coordinates;
            // Generate street name
            char street_name = char('a' + i);
            // Generate the number of segments
            urandom.read((char*)&segment, 1);
            segment = 1 + segment % n;
            for(int j = 0; j <= segment; ++j) {
                // Define a vector to store current coordinate
                std::vector<int> coord(2);
                // Generate x value of current coordinate
                urandom.read((char*)&co_range, 1);
                co_range = co_range % (2 * c + 1);
                coord[0] = co_range - c;
                // Generate y value of current coordinate
                urandom.read((char*)&co_range, 1);
                co_range = co_range % (2 * c + 1);
                coord[1] = co_range - c;
                // If the newly generated segment is valid then add it to the vector of segments
                if(CheckSegment(coordinates, coord)) {
                    coordinates.push_back(coord);
                    count_seg = 30;
                }
                // If the newly generated segment is not valid then regenerate the segment
                else {
                    --j;
                    --count_seg;
                    // std::cerr << "seg regenerate" << count_seg << "\n";
                    if(count_seg == 0) {
                        std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts!" << "\n";
                        exit(1);
                    }
                }    
            }
            // If the newly generated street is valid then add it to the vector of streets
            if(CheckStreet(street_coord, coordinates, intersect)) {
                street_names.push_back(street_name);
                street_coord.push_back(coordinates);
                count_street_old = count_street;
                count_street = 25;
                // Check if there is intersection between streets while reaching the last street
                if(i == street - 1 && *intersect == 0) {
                    // std::cerr << "No intersection" <<  "\n";
                    street_names.pop_back();
                    street_coord.pop_back();
                    count_street = count_street_old;
                    --i;
                    --count_street;
                    // std::cerr << "street regenerate" << count_street << "\n";
                    if(count_street == 0) {
                        std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts!" << "\n";
                        exit(1);
                    }
                }
            }
            // If the newly generated street is not valid then regenerate the street
            else {
                --i;
                --count_street;
                // std::cerr << "street regenerate" << count_street << "\n";
                if(count_street == 0) {
                    std::cerr << "Error: failed to generate valid input for 25 simultaneous attempts!" << "\n";
                    exit(1);
                }
            }
        }
        PrintCommand(street_names, street_coord);
        // Generate the interval before next iteration
        urandom.read((char*)&delay, 1);
        delay = 5 + delay % (l - 4);
        sleep(delay);
    }
    urandom.close();
}


int main(int argc, char **argv) {
    int s = 10, n = 5, l = 5, c = 20;
    int para;
    std::string svalue, nvalue, lvalue, cvalue;
    bool error_flag = false;
    // Parse the command line parameters
    while((para = getopt(argc, argv, "s:n:l:c:")) != -1) {
        if(para == 's') {
            svalue = optarg;
            s = atoi(svalue.c_str());
            if(s < 2) {
                std::cerr << "Error: Invalid value of s!" << "\n";
                error_flag = true;
                break;
            }
            continue;
        }
        if(para == 'n') {
            nvalue = optarg;
            n = atoi(nvalue.c_str());
            if(n < 1) {
                std::cerr << "Error: Invalid value of n!" << "\n";
                error_flag = true;
                break;
            }
            continue;
        }
        if(para == 'l') {
            lvalue = optarg;
            l = atoi(lvalue.c_str());
            if(l < 5) {
                std::cerr << "Error: Invalid value of l!" << "\n";
                error_flag = true;
                break;
            }
            continue;
        }
        if(para == 'c') {
            cvalue = optarg;
            c = atoi(cvalue.c_str());
            if(c < 1) {
                std::cerr << "Error: Invalid value of c!" << "\n";
                error_flag = true;
                break;
            }
            continue;
        }
        else {
            std::cerr << "Unrecognized parameter: " << (char)para << "\n";
            error_flag = true;
            break;
        }
    }
    // If all the parameters are valid then run the generator function
    if(!error_flag) {
        PyGenerator(s, n, l, c);  
    }        
    return 0;
}

