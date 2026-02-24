import json

def generate_graph_with_edges(input_file, output_file):
    print(f">>> Reading data from {input_file}...")
    
    with open(input_file, 'r', encoding='utf-8') as f:
        data = json.load(f)

    graph_data = {}

    for slug, nodes in data.items():
        print(f"Processing roadmap: {slug}")
        
        # Initialize Adjacency List for this roadmap
        # Structure: { "NodeA": ["NodeB", "NodeC"], ... }
        adj_list = {}
        
        # Logic: Create a linear dependency for demo
        # Example: Node[0] -> Node[1] -> Node[2]
        for i in range(len(nodes) - 1):
            current_skill = nodes[i]['skill']
            next_skill = nodes[i+1]['skill']
            
            if current_skill not in adj_list:
                adj_list[current_skill] = []
            
            # Add edge: current_skill is a prerequisite for next_skill
            adj_list[current_skill].append(next_skill)
            
        # Ensure the last node is also in the dictionary
        if nodes and nodes[-1]['skill'] not in adj_list:
            adj_list[nodes[-1]['skill']] = []

        graph_data[slug] = adj_list

    # Save the structured graph
    with open(output_file, 'w', encoding='utf-8') as f:
        json.dump(graph_data, f, ensure_ascii=False, indent=4)
    
    print(f"\n>>> Success! Graph saved to '{output_file}'")

if __name__ == "__main__":
    # Use the file from previous step
    generate_graph_with_edges('mega_dataset.json', 'roadmap_graph.json')