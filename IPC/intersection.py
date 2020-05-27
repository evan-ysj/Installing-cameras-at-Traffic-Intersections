#/usr/bin/python2.7
import sys
import re

class TrafficGraph(object):
    def __init__(self):
        # store street information with a dictionary
        self.street_info = dict()
        # store initial street information with a dictionary
        self.street_info_init = dict()
        # store vertices with a list after generating a graph
        self.vertex = set()
        # store edges with a list after generating a graph
        self.edge = set()
        # a flag used to mark whether the "street_info" is changed
        self.is_changed = True
    
    def addStreet(self, key, value):
        self.street_info_init[key] = value
        self.street_info[key] = list(value)
        self.is_changed = True
    
    def changeStreet(self, key, value):
        #if one item is changed, recover the "street_info" first
        for k, v in self.street_info_init.items():
            self.street_info[k] = list(v)  
        self.addStreet(key, value)
    
    def removeStreet(self, key):
        for k, v in self.street_info_init.items():
            self.street_info[k] = list(v)       
        del self.street_info[key]
        del self.street_info_init[key]
        self.is_changed = True
    
    def generate(self):
        # there is no need to regenerate a graph if the "street_info" is not changed
        if not self.is_changed:
            return
        # if the info is changed clear "vertex" and "edge" to regenerate a new graph
        self.vertex.clear()
        self.edge.clear()
        for k1, v1 in self.street_info.items():
            for k2, v2 in self.street_info.items():
                if k2 == k1:
                    continue
                # for each line in two streets we try to find the intersection 
                # and put proper vertices and edges into "vertex" and "edge"
                for line1 in v1:
                    for line2 in v2:
                        if line1 not in v1:
                            continue
                        intersect = self._cross(line1, line2)
                        # if line1 and line2 do not have intersection we go on with another two lines
                        if len(intersect) == 0:
                            continue 
                            
                        # this part tries to find out whether line1 and line2 will be separated by intersection
                        # if so we remove the line from street_info for we have to use two sublines in the future
                        # if the line lies in "edge" we also have to remove it because it is invalid now
                        l1 = self._formEdge(line1[0], line1[1])                        
                        if intersect != line1[0] and intersect != line1[1]: 
                            if l1 in self.edge:
                                self.edge.remove(l1)
                            v1.remove(line1)
                        l2 = self._formEdge(line2[0], line2[1])                        
                        if intersect != line2[0] and intersect != line2[1]: 
                            if l2 in self.edge:
                                self.edge.remove(l2)
                            if line2 in v2:
                                v2.remove(line2)
                                
                        # check the intersection and four verteices of line1 and line2
                        # if the vetices and edges are valid then put them into corresponding sets
                        # and put the new separated lines into street_info
                        for point in (intersect, line1[0], line1[1], line2[0], line2[1]):
                            new_vertex = (('{0:.2f}'.format(point[0]) + '_' + '{0:.2f}'.format(point[1])), point)
                            self.vertex.add(new_vertex)
                            if point != intersect:
                                e = self._formEdge(intersect, point)
                                self.edge.add(e)
                                new_line = (min(intersect, point), max(intersect, point))
                                if point in line1 and new_line not in v1:
                                    v1.append(new_line)
                                if point in line2 and new_line not in v2:
                                    v2.append(new_line)       
        self.is_changed = False
    
    # compute the intersection of two lines, return empty tuple if there is none
    @staticmethod
    def _cross(line1, line2):
        l1p1 = min(line1[0], line1[1])
        l1p2 = max(line1[0], line1[1])
        l2p1 = min(line2[0], line2[1])
        l2p2 = max(line2[0], line2[1])
        intersect = ()
        k1 = k2 = b1 = b2 = 0
        if l1p1[0] == l1p2[0] and l2p1[0] == l2p2[0]:
            if l1p1 == l2p2:
                intersect = l1p1
            elif l1p2 == l2p1:
                intersect = l1p2
            return intersect
        elif l1p1[0] == l1p2[0]:
            k2 = float((l2p1[1] - l2p2[1]) / (l2p1[0] - l2p2[0]))
            b2 = float((l2p1[0] * l2p2[1] - l2p2[0] * l2p1[1]) / (l2p1[0] - l2p2[0]))
            x = round(l1p1[0], 2)
            y = round(k2 * x + b2, 2)            
            if y >= l1p1[1] and y <= l1p2[1] and x >= l2p1[0] and x <= l2p2[0]:
                if x == 0:
                    x = 0.0
                if y == 0:
                    y = 0.0
                intersect = (x, y)
        elif l2p1[0] == l2p2[0]:
            k1 = float((l1p1[1] - l1p2[1]) / (l1p1[0] - l1p2[0]))
            b1 = float((l1p1[0] * l1p2[1] - l1p2[0] * l1p1[1]) / (l1p1[0] - l1p2[0]))
            x = round(l2p1[0], 2)
            y = round(k1 * x + b1, 2)
            if y >= l2p1[1] and y <= l2p2[1] and x >= l1p1[0] and x <= l1p2[0]:
                if x == 0:
                    x = 0.0
                if y == 0:
                    y = 0.0
                intersect = (x, y)
        else: 
            k1 = float((l1p1[1] - l1p2[1]) / (l1p1[0] - l1p2[0]))
            b1 = float((l1p1[0] * l1p2[1] - l1p2[0] * l1p1[1]) / (l1p1[0] - l1p2[0]))
            k2 = float((l2p1[1] - l2p2[1]) / (l2p1[0] - l2p2[0]))
            b2 = float((l2p1[0] * l2p2[1] - l2p2[0] * l2p1[1]) / (l2p1[0] - l2p2[0]))
            if k1 == k2:
                if l1p1 == l1p2:
                    intersect = l1p1
                elif l1p2 == l2p1:
                    intersect = l1p2
                return intersect
            x = round((b2 - b1) / (k1 - k2), 2)
            y = round((k1 * b2 - k2 * b1) / (k1 - k2), 2)
            if x >= l1p1[0] and x <= l1p2[0] and x >= l2p1[0] and x <= l2p2[0] and y >= min(l1p1[1], l1p2[1]) and y <= max(l1p1[1], l1p2[1]) and y >= min(l2p1[1], l2p2[1]) and y <= max(l2p1[1], l2p2[1]):
                if x == 0:
                    x = 0.0
                if y == 0:
                    y = 0.0
                intersect = (x, y)
#        print line1, line2, '---', intersect
        return intersect
    
    # compute the valid edge formation of two vertices
    def _formEdge(self, point1, point2):
        p1 = min(point1, point2)
        p2 = max(point1, point2)
        id1 = '{0:.2f}'.format(p1[0]) + '_' + '{0:.2f}'.format(p1[1])
        id2 = '{0:.2f}'.format(p2[0]) + '_' + '{0:.2f}'.format(p2[1])
        return (id1, id2)

    
# check whether the input is valid
def inputCheck(inputs, graph_obj):
    if inputs[0] == 'a' or inputs[0] == 'c':
        if len(inputs) < 3:
            alertFormatError()
            return False
        match = re.match(r'^[a-zA-Z][a-zA-Z\s]*', inputs[1])
        inputs[1] = inputs[1].lower()
        if not match or match.span()[1] < len(inputs[1]):
            alertFormatError()
            return False            
        if inputs[0] == 'a' and inputs[1] in graph_obj.street_info:
            sys.stderr.write('Error: Specified street already exits\n')
            return False
        if inputs[0] == 'c' and inputs[1] not in graph_obj.street_info:
            sys.stderr.write('Error: Specified street does not exit\n')
            return False
        for t in re.split(r'\s+', inputs[2]):
            if not re.match(r'(\([-]?\d+[\.\d+]*,[-]?\d+[\.\d+]*\)\s*)+', t):
                alertFormatError()
                return False
        return True           
    elif inputs[0] == 'r':
        if len(inputs) > 2:
            alertFormatError()
            return False
        if len(inputs) == 1:
            return True
        match = re.match(r'^[a-zA-Z][a-zA-Z\s]*', inputs[1])
        inputs[1] = inputs[1].lower()
        if not match or match.span()[1] < len(inputs[1]):
            alertFormatError()
            return False       
        if inputs[1] not in graph_obj.street_info:
            sys.stderr.write('Error: Specified street does not exit\n')
            return False
        return True    
    elif inputs[0] == 'g':
        if len(inputs) > 1:
            alertFormatError()
            return False
        return True
    else:
        alertFormatError()
        return False

    
def alertFormatError():
    sys.stderr.write('Error: Incorrect input format!\n')

    
# parse the coordinate into valid format
def parseCoordinate(coordinate):
    street_vertex = re.split(r'\)\s*\(', coordinate[1:-1])
    street_lines = []
    for i in range(len(street_vertex) - 1):
        t1 = street_vertex[i].split(',')
        t2 = street_vertex[i+1].split(',')
        try:
            t10 = round(float(t1[0]), 2)
            t11 = round(float(t1[1]), 2)
            t20 = round(float(t2[0]), 2)
            t21 = round(float(t2[1]), 2)
            p1 = min((t10, t11), (t20, t21))
            p2 = max((t10, t11), (t20, t21))
            street_lines.append((p1, p2))
        except:
            alertFormatError()
            return []
    if len(street_lines) < 1:
        sys.stderr.write('Error: There should be at least two coordinates\n')
        return []
    for i in range(len(street_lines)):
        for j in range(len(street_lines)):
            if i == j:
                continue
            if street_lines[i] == street_lines[j]:
                sys.stderr.write('Error: Repeated coordinates\n')
                return []
    return street_lines
    

def main():
    graph = TrafficGraph()
    while True:
        try:
            command = sys.stdin.readline()
            # process the input string
            command = command.strip()
            if command == '':
                continue
            arguments = re.split(r'"', command)
            for i in range(len(arguments)):
                arguments[i] = arguments[i].strip()
            arguments = filter(None, arguments)
        
            # check the input
            if not inputCheck(arguments, graph):
                continue
            
            # parse the input coordinate and get the output
            if arguments[0] == 'a' or arguments[0] == 'c':
                value = parseCoordinate(arguments[2])
                key = arguments[1].lower()
                if not value:
                    continue
                if arguments[0] == 'a':
                    graph.addStreet(key, value)
                if arguments[0] == 'c':
                    graph.changeStreet(key, value)
            elif arguments[0] == 'r':
                if len(arguments) == 1:
                    graph.street_info.clear()
                    graph.street_info_init.clear()
                    graph.vertex.clear()
                    graph.edge.clear()
                else:
                    graph.removeStreet(arguments[1])
            else:
                #break
        
                graph.generate()
                if graph.edge and graph.vertex:
                    id = {}
                    val = 0
                    for v in graph.vertex:
                        id[v[0]] = val
                        val += 1
                    outputstr1 = 'V {0}\n'.format(val)
                    outputstr2 = 'E {'
                    for i, e in enumerate(graph.edge):
                        if i == len(graph.edge) - 1:
                            outputstr2 += '<{0},{1}>'.format(id[e[0]], id[e[1]])
                        else:
                            outputstr2 += '<{0},{1}>,'.format(id[e[0]], id[e[1]])
                    outputstr2 += '}\n'
                    sys.stdout.write(outputstr1)
                    sys.stdout.write(outputstr2)
                    sys.stdout.flush()
                       
        except (EOFError, KeyboardInterrupt, SystemExit):
            break
    sys.exit(0)

    
if __name__ == '__main__':
    main()

