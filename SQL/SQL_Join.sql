/*
Find out what 10 countries are closest to Argentina as per the following criterion:
C = ABS((GNP% + APC%) - (GNP_Argentina% + APC_Argentina%)) 
GNP% - GNP of a country in percent out of to the biggest possible GNP in the world.
APC% - APC of a country in percent out of to the biggest possible APC in the world.
APC – SurfaceArea/Population. It is the area per one citizen.
Hint. Use JOIN operators to reach the desired result. Add condition on population to suppress the division by zero errors.
To apply cross join just write CROSS JOIN like you would write it with usual theta-join but do not add the ON keyword.
*/

-- Solution
-- Calc new metrics: GNP%, APC%
with t as (
select
	name, 
	100.0 * gnp / (
	-- Find max GNP
	select
		gnp
	from
		country
	order by
		gnp desc
	limit 1 ) as gnp_p, 
	100.0 * surfacearea / population / (
	-- Find max APC
	select
		surfacearea / population as apc
	from
		country
	where
		population != 0
	order by
		apc desc
	limit 1) as apc_p
from
	country
where
	population != 0)
select 
	t1.name as "Country name",
	t1.gnp_p as "GNP% for country",
	t1.apc_p as "APC% for country",
	t2.gnp_p as "GNP% for Argentina",
	t2.apc_p as "APC% for Argentina",
	-- Calc new metric: C
	abs((t1.gnp_p + t1.apc_p) - (t2.gnp_p + t2.apc_p)) as "C"
from
	t as t1
-- Combine each country (except Argentina itself) with Argentina (stack columns horizontally)
cross join t as t2
where
	t2.name = 'Argentina'
	and t1.name != 'Argentina'
-- Sort in ascending order by C and get top10 similar countries
order by
	"C" asc
limit 10;
