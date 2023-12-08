import urllib.parse
import requests
import time
import inspect
from typing import Optional
from bs4 import BeautifulSoup
from fake_useragent import UserAgent
import matplotlib.pyplot as plt
import matplotlib.cm
import networkx as nx

BASE_URL = 'https://www.imdb.com/'
HEADERS = {'Accept-Language': 'en', 'User-Agent': UserAgent().chrome}
session = requests.Session()
session.headers.update(HEADERS)


def get_absolute_url(path: str) -> Optional[str]:
    ''' Return full (absolute) URL by combining the base URL with another URL (path) or None if unsuccessful. '''
    try:
        return urllib.parse.urljoin(BASE_URL, path)
    except Exception as e:
        print(f'{inspect.stack()[0][3]}: {repr(e)}')
        return None


def get_soup_from_url(url: str) -> Optional[BeautifulSoup]:
    ''' Return a BeautifulSoup object from URL or None if unsuccessful. '''
    try:
        response = session.get(url)
        time.sleep(0.1)
        response.raise_for_status()
        soup = BeautifulSoup(response.text, 'lxml')
        #print('Request is successful. BeautifulSoup object is ready.')
        return soup
    except Exception as e:
        print(f'{inspect.stack()[0][3]}: {repr(e)}')
        return None


def format_str(str, sep):
    ''' Replace whitespaces in a string with <sep> char. '''
    return f'{sep}'.join(str.split())


def get_plot_attrs(
                    G: nx.Graph, 
                    distance: Optional[int]=None
                    ) -> tuple[list, list, list, dict]:
    '''
    Prepare edge attributes for plotting the distance graph's edges.
    
    Parameters
    ----------
    G : nx.Graph
        NetworkX graph for which to prepare edge attributes.
    distance : int of float('inf')
        Specific distance of the edge between two actors to show or None to show all edges.

    Returns
    -------
    Tuple of three lists and one dictionary with edge attributes to be used in plotting.
    '''
    edges_dict = {k: G.edges()[k]['distance'] for k in G.edges()}
    if distance:
        edges_for_distance = dict(filter(lambda x: x[1] == distance, edges_dict.items()))
    else:
        edges_for_distance = edges_dict
    edgelist = list(edges_for_distance.keys())
    edge_color = list(edges_for_distance.values())
    edge_width = [2/d for d in list(edges_for_distance.values())]
    edge_labels = edges_for_distance
    return (edgelist, edge_color, edge_width, edge_labels)


def plot_graph(
                G: nx.Graph, 
                plot_name: str, 
                edgelist: list, 
                edge_color: list, 
                edge_width: list, 
                edge_labels: dict, 
                nlist: Optional[list]=None,
                rad: Optional[float]=None,
                save_to_file: bool=False
                ) -> None:
    '''
    Plot the distance graph.
    
    Parameters
    ----------
    G : nx.Graph
        NetworkX graph to plot.
    plot_name : str
        Plot's name (title). 
    edgelist : list
        List of edges. Each edge is a tuple (actor1, actor2).
    edge_color : list
        List of color codes in the colormap 'hsv', as per the distance. Distance 1 edges are red, distance 2 edges are green etc.
    edge_width : list
        List of edge widths, as per the distance. Distance 1 edges are the thickest.
    edge_labels : dict
        Dict of edge labels where keys are edge tuples (actor1, actor2) and values are edge distances.
    nlist : list
        The node list sorted as needed for specific display order, or None for default node display order.
    rad : float
        The angle in radians of the first (start) node or None for default angle, examples:
        0     => -.
        pi/2  => i
        pi    => .-
        3pi/2 => !
    save_to_file : bool
        Whether to save the plot to a png file. False by default.

    Returns
    -------
    None, just plots the graph.
    '''
    
    layout = nx.shell_layout(G, rotate=rad, nlist=nlist)
    plt.figure(figsize=(8, 6))
    plt.title(f'{G.name}: {plot_name}')
    nx.draw_networkx_nodes(
        G,
        node_color='xkcd:light blue', 
        node_size=3000, 
        node_shape='o',
        edgecolors='xkcd:purple',
        pos=layout
        )
    nx.draw_networkx_labels(
        G,
        font_color='black',
        font_weight='normal',
        font_size=10,
        pos=layout
        )
    nx.draw_networkx_edges(
        G,
        arrows=True,
        arrowsize=14,
        edgelist=edgelist,
        edge_color=edge_color,
        edge_cmap=matplotlib.cm.get_cmap('hsv'),
        edge_vmin=1.0,
        edge_vmax=4.0,
        width=edge_width,
        node_size=3000,
        pos=layout
        )
    nx.draw_networkx_edge_labels(
        G,
        edge_labels=edge_labels,
        font_color='black',
        rotate=False,
        label_pos=0.5,
        verticalalignment='center',
        pos=layout
        )
    # Blue box around plot
    spines = plt.gca().spines
    for axis in ['top', 'bottom', 'left', 'right']:
        spines[axis].set_linewidth(0.25)
        spines[axis].set_color('C0')
    plt.tight_layout()
    if save_to_file:
        plt.savefig(f"distance_graph_{format_str(plot_name, sep='_')}.png");
    return None


def actor_name_to_filename(
                            actor_name: str,
                            prepend_name: str='movie_desc_for_'
                            ) -> str:
    ''' 
    Convert actor's name string to the filename with their movies descriptions of the format <prepend_name><actor_name>.txt. 
    
    Parameters
    ----------
    actor_name : str
        Actor's name.
    prepend_name : str
        Prefix to use in the filename. Default is 'movie_desc_for_'.
    
    Returns
    -------
    Formatted filename as a string.
    '''

    return f"{format_str(prepend_name + actor_name, sep='_')}.txt"


def save_actor_descriptions_to_file(
                                    actor_name: str, 
                                    actor_descriptions: list[str]
                                    ) -> None:
    '''
    Save a list of all actor's movies descriptions to a text file with filename <prepend_name><actor_name>.txt. 
    
    Parameters
    ----------
    actor_name : str
        Actor's name.
    actor_descriptions : list[str]
        All actor's movies descriptions.
    
    Returns
    -------
    None, just saves to file.
    '''
    
    filename = actor_name_to_filename(actor_name)
    with open(filename, 'w') as f:
        f.writelines(actor_description + '\n' for actor_description in actor_descriptions)
    return None


def read_actor_descriptions_from_file(actor_name: str) -> list[str]:
    '''
    Read all actor's movies descriptions from a text file with filename <prepend_name><actor_name>.txt.
    Return a list of strings constructed from this file, where each element is a short description of a movie, where the actor played.

    Parameters
    ----------
    actor_name : str
        Actor's name.
    
    Returns
    -------
    List of strings -- all actor's movies descriptions.
    '''

    filename = actor_name_to_filename(actor_name)
    with open(filename, 'r') as f:
        actor_descriptions = f.readlines()
    return actor_descriptions


def get_text_from_actor_descriptions(actor_descriptions: list[str]) -> str:
    '''
    Construct one text from the list of all actor's movies descriptions.

    Parameters
    ----------
    actor_descriptions : list[str]
        All actor's movies descriptions.
    
    Returns
    -------
    One text from all actor's movies descriptions as a string.
    '''
    raw_actor_text = ''
    for actor_description in actor_descriptions:
        actor_description = actor_description.strip('\n')
        raw_actor_text = f'{raw_actor_text}{actor_description} '
    # Clean the raw text by keeping only letters in words
    actor_text = ''
    for word in raw_actor_text.split():
        word = ''.join(filter(str.isalpha, word))
        actor_text = f'{actor_text}{word} '
    return actor_text


if __name__ == '__main__':
    raise RuntimeError('Module should not be run as a standalone!')
