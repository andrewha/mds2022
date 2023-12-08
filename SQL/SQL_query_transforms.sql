/* The first query:
The following query finds cities with high population (>=8m) which are not capitals. 
Rewrite this query without a subquery.
select * 
from city as ct
where not exists (
					select 1 
					from country as c 
					where c.capital = ct.id
					)
and population >= 8000000;
*/

-- Solution: use join
select ct.*
from city as ct
-- Combine with country to exclude capitals
join country as c on c.code = ct.countrycode and ct.id != c.capital 
-- Apply condition (selection)
where ct.population >= 8000000;

/* Result (same as for the initial query: 6 rows)
id  |name           |countrycode|district   |population|
----+---------------+-----------+-----------+----------+
 206|São Paulo      |BRA        |São Paulo  |   9968485|
1024|Mumbai (Bombay)|IND        |Maharashtra|  10500000|
1890|Shanghai       |CHN        |Shanghai   |   9696300|
2822|Karachi        |PAK        |Sindh      |   9269265|
3357|Istanbul       |TUR        |Istanbul   |   8787958|
3793|New York       |USA        |New York   |   8008278|
*/

/* The second query:
You have a query to find cities that have population greater than average city population in the same country.
Rewrite this query using a derived table in FROM. The new version of the query should not include a subquery in where.
select c.name countryname, ct.name cityname, ct.population as city_population
from city as ct 
join country as c on c.code = ct.countrycode 
where ct.population > (
						select avg(population) as avg_population 
						from city 
						where countrycode = ct.countrycode
						)
order by c."name" , ct."name";
*/

-- Solution: prepare a look-up table with average city population for each country using group by
-- This query works faster, since the look-up aggregation is calculated only once
select
	c.name as countryname,
	ct.name as cityname,
	ct.population as city_population
from
	-- Create a look-up table with average city population for each country code
	(
	select
		countrycode,
		avg(population) as avg_ct_pop
	from city
	group by countrycode
	) as country_avg_city_pop
-- Combine with city to get population for each city
join city as ct on ct.countrycode = country_avg_city_pop.countrycode
-- Combine with country to get name for each country
join country as c on c.code = ct.countrycode
-- Apply condition (selection) without any subquery
where ct.population > country_avg_city_pop.avg_ct_pop
order by c.name, ct.name;

/* Result (same as for the initial query: 915 rows)
countryname                          |cityname                      |city_population|
-------------------------------------+------------------------------+---------------+
Afghanistan                          |Kabul                         |        1780000|
Algeria                              |Alger                         |        2168000|
Algeria                              |Constantine                   |         443727|
Algeria                              |Oran                          |         609823|
American Samoa                       |Tafuna                        |           5200|
...                                  |...                           |            ...|
Yemen                                |Taizz                         |         317600|
Yugoslavia                           |Beograd                       |        1204000|
Zambia                               |Lusaka                        |        1317000|
Zimbabwe                             |Bulawayo                      |         621742|
Zimbabwe                             |Harare                        |        1410000|
*/
