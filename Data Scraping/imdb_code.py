from typing import Optional
import urllib.parse
import re
import inspect
from bs4 import BeautifulSoup
import networkx as nx
from tqdm.notebook import tqdm
from imdb_helper_functions import get_absolute_url, get_soup_from_url

# Store cache for all collected Beautiful soup objects
soups = {}


def get_movies_by_actor_soup(
                            actor_page_soup: BeautifulSoup, 
                            num_of_movies_limit: Optional[int]=None, 
                            titles_only: bool=False, 
                            absolute_url: bool=False, 
                            sort_result: bool=False
                            ) -> list[tuple]:
    '''
    Find all movies from the BeautifulSoup object for:
    - only those films, that have been already released,
    - both male and female actors, 
    - where the current actor was involved as an actor, and not as somebody else (director, producer), 
    - and only full feature films, and not something else (like TV series).

    Parameters
    ----------
    actor_page_soup : BeautifulSoup
        A BeautifulSoup object to parse.
    num_of_movies_limit : int
        Number of movies to return. Default is None which will return all found movies.
    titles_only : bool
        Whether to return a list of movie titles only, without paths.
    absolute_url : bool
        Whether to return absolute URL or just path (relative URL).
    sort_result : bool
        Whether to sort the result list in alphabetic order.

    Returns
    -------
    List of tuples (movie's title, path or absolute url).
    '''
    
    assert isinstance(actor_page_soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
    assert num_of_movies_limit is None or (isinstance(num_of_movies_limit, int) and num_of_movies_limit > 0), f'{inspect.stack()[0][3]}: Movies limit must be None for no limit, or a positive integer!'
    movies = []
    # Find <div class="filmo-row odd | even" id="actor... | actress..."> -- this the actor's / actress's filmography as an actor
    actor_filmo = actor_page_soup.find_all('div', class_=['filmo-row odd', 'filmo-row even'], id=re.compile(r'actor|actress'))
    for movie in actor_filmo:
        # Skip unreleased movies
        if movie.find_all(class_='in_production'):
            continue
        # Skip non-feature movies. i.e. any movie div has some text in the 4th element of contents, such as (TV Series), (Video Game), (Music Video) etc.
        if not movie.contents[4].strip():
            movie_link = movie.find('a')
            movie_title = movie_link.text
            movie_path = urllib.parse.urlparse(movie_link['href']).path
            if absolute_url:
                movie_url = get_absolute_url(movie_path)
            else:
                movie_url = movie_path
            if titles_only:
                movies.append(movie_title)
            else:
                movies.append((movie_title, movie_url))
        if num_of_movies_limit:
            if len(movies) == num_of_movies_limit:
                break
    if sort_result:
        movies = sorted(movies)
    return movies


def get_actors_by_movie_soup(
                            cast_page_soup: BeautifulSoup, 
                            num_of_actors_limit: Optional[int]=None, 
                            names_only: bool=False, 
                            absolute_url: bool=False, 
                            sort_result: bool=False
                            ) -> list[tuple]:
    '''
    Find all actors from the BeautifulSoup object for:
    - only actors (not some other crew members),
    - both male and female actors.

    Parameters
    ----------
    cast_page_soup : BeautifulSoup
        A BeautifulSoup object to parse.
    num_of_actors_limit : int
        Number of actors to return. Default is None which will return all found actors.
    names_only : bool
        Whether to return a list of actor names only, without paths.
    absolute_url : bool
        Whether to return absolute URL or just path (relative URL).
    sort_result : bool
        Whether to sort the result list in alphabetic order.
    
    Returns
    -------
    List of tuples (actor's name, path or absolute url).
    '''
    
    assert isinstance(cast_page_soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
    assert num_of_actors_limit is None or (isinstance(num_of_actors_limit, int) and num_of_actors_limit > 0), f'{inspect.stack()[0][3]}: Actors limit must be None for no limit, or a positive integer!'
    actors = []
    # Find the only <table class="cast_list"> and then all <td class="primary_photo"> within this table
    cast = cast_page_soup.find_all('table', class_=['cast_list'])[0].find_all('td', class_='primary_photo')
    for actor in cast:
        actor_link = actor.find('a')
        actor_name = actor_link.find('img')['alt']
        actor_path = urllib.parse.urlparse(actor_link['href']).path
        if absolute_url:
            actor_url = get_absolute_url(actor_path)
        else:
            actor_url = actor_path
        if names_only:
            actors.append(actor_name)
        else:
            actors.append((actor_name, actor_url))
        if num_of_actors_limit:
            if len(actors) == num_of_actors_limit:
                break
    if sort_result:
        actors = sorted(actors)
    return actors


def get_movie_distance(
                        actor_start_url: str, 
                        actor_end_url: str, 
                        num_of_actors_limit: Optional[int]=None, 
                        num_of_movies_limit: Optional[int]=None
                        ) -> int:
    '''
    Find pairwise distance between two actors.

    Parameters
    ----------
    actor_start_url : str
        Actor's URL to their filmography page from who we are finding a path.
    actor_end_url : str
        Actor's URL to their filmography page to whom we are finding a path.
    num_of_actors_limit : int
        Number of actors to return. Default is None which will return all found actors.
    names_only : bool
        Whether to return a list of actor names only, without paths.
    
    Returns
    -------
    Distance as int, if there is a path between two actors, of float('inf') if no path.
    '''
    
    assert num_of_movies_limit is None or (isinstance(num_of_movies_limit, int) and num_of_movies_limit > 0), f'{inspect.stack()[0][3]}: Movies limit must be None for no limit, or a positive integer!'
    assert num_of_actors_limit is None or (isinstance(num_of_actors_limit, int) and num_of_actors_limit > 0), f'{inspect.stack()[0][3]}: Actors limit must be None for no limit, or a positive integer!'
    # Graph to store actors (nodes) and their relations (edges)
    G = nx.Graph()
    # Cache soup objects
    if not actor_start_url in soups:
        soup = get_soup_from_url(actor_start_url)
        assert isinstance(soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
        soups[actor_start_url] = soup
    if not actor_end_url in soups:
        soup = get_soup_from_url(actor_end_url)
        assert isinstance(soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
        soups[actor_end_url] = soup
    # Level 1 relation
    # No need to limit the number of movies here, since soup objects are already loaded,
    # no new GET requests are required, and we want to increase the chance of actors' filmography intersection
    actor_start_movies = get_movies_by_actor_soup(soups[actor_start_url], absolute_url=True)
    actor_end_movies = get_movies_by_actor_soup(soups[actor_end_url], absolute_url=True)
    same_movies = set(actor_start_movies).intersection(set(actor_end_movies))
    if same_movies:
        G.add_edge(actor_start_url, actor_end_url)
        distance = nx.dijkstra_path_length(G, source=actor_start_url, target=actor_end_url)
        return distance
    # Level 2 relation
    # Kinda two-pointer approach
    # Limit the number of movies to decrease the number of GET requests
    for actor_start_movie, actor_end_movie in zip(
                                                    actor_start_movies[:num_of_movies_limit], 
                                                    actor_end_movies[:num_of_movies_limit]
                                                    ):
        # Cache soup objects
        if not f'{actor_start_movie[1]}fullcredits' in soups:
            soup = get_soup_from_url(f'{actor_start_movie[1]}fullcredits')
            assert isinstance(soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
            soups[f'{actor_start_movie[1]}fullcredits'] = soup
        if not f'{actor_end_movie[1]}fullcredits' in soups:
            soup = get_soup_from_url(f'{actor_end_movie[1]}fullcredits')    
            assert isinstance(soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
            soups[f'{actor_end_movie[1]}fullcredits'] = soup
        # Kinda two-pointer approach
        # Limit the number of actors to decrease the number of GET requests
        for actor_1, actor_2 in zip(
                        get_actors_by_movie_soup(soups[f'{actor_start_movie[1]}fullcredits'], num_of_actors_limit, absolute_url=True),
                        get_actors_by_movie_soup(soups[f'{actor_end_movie[1]}fullcredits'], num_of_actors_limit, absolute_url=True)
                        ):
            if (actor_1[1] in G.nodes()) and (actor_2[1] in G.nodes()):
                continue
            if not actor_1[1] in G.nodes():
                G.add_edge(actor_start_url, actor_1[1])
            if not actor_2[1] in G.nodes():
                G.add_edge(actor_end_url, actor_2[1])
            # Cache soup objects
            if not f'{actor_1[1]}fullcredits' in soups:
                soup = get_soup_from_url(f'{actor_1[1]}fullcredits')
                assert isinstance(soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
                soups[f'{actor_1[1]}fullcredits'] = soup
            if not f'{actor_2[1]}fullcredits' in soups:
                soup = get_soup_from_url(f'{actor_2[1]}fullcredits')
                assert isinstance(soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
                soups[f'{actor_2[1]}fullcredits'] = soup
            # No need to limit the number of movies here, since soup objects are already loaded,
            # no new GET requests are required, and we want to increase the chance of actors' filmography intersection
            actor_1_movies = get_movies_by_actor_soup(soups[f'{actor_1[1]}fullcredits'], absolute_url=True)
            actor_2_movies = get_movies_by_actor_soup(soups[f'{actor_2[1]}fullcredits'], absolute_url=True)
            same_movies_1 = set(actor_1_movies).intersection(set(actor_end_movies))
            if same_movies_1:
                G.add_edge(actor_1[1], actor_end_url)
                distance = nx.dijkstra_path_length(G, source=actor_start_url, target=actor_end_url)
                return distance
            same_movies_2 = set(actor_2_movies).intersection(set(actor_start_movies))
            if same_movies_2:
                G.add_edge(actor_2[1], actor_start_url)
                distance = nx.dijkstra_path_length(G, source=actor_start_url, target=actor_end_url)
                return distance
    # Level 3+ relation
    distance = 3
    return distance


def get_movie_descriptions_by_actor_soup(actor_page_soup: BeautifulSoup) -> list[str]:
    '''
    Return a list of strings, where each element is a short description of a movie, where the actor played.
    Every movie has such a description on its web page.

    Parameters
    ----------
    actor_page_soup : BeautifulSoup
        A BeautifulSoup object to parse.
    
    Returns
    -------
    List of strings -- all actor's movies descriptions.
        
    '''
    assert isinstance(actor_page_soup, BeautifulSoup), f'{inspect.stack()[0][3]}: Expecting a BeautifulSoup object!'
    actor_descriptions = []
    actor_movies = get_movies_by_actor_soup(actor_page_soup, absolute_url=True)
    for movie_url in tqdm(actor_movies):
        movie_soup = get_soup_from_url(movie_url[1])
        # Find <p data-testid="plot">
        actor_description = movie_soup.find_all('p', attrs={'data-testid': 'plot'})[0].find_all('span')[2].text
        actor_descriptions.append(actor_description)
    return actor_descriptions
    

if __name__ == '__main__':
    raise RuntimeError('Module should not be run as a standalone!')
