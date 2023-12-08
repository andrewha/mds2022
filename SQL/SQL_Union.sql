/*
Write a query that returns the following table showing data about two governmental forms - 'Republic' and 'Federal republic. The last column shows the others.
GLE = greatest lifetime expectancy
LLE = least lifetime expectancy
The "Others" column should contain results that relate to all government forms except 'Republic' and 'Federal republic'.
In this task you should use the following operations to prepare a query:
union
order by with limit
filtering in where
Aggregation or window functions should not be used.
*/

-- Solution
-- Calc the first row: GLE
(
select
	'GLE' as "Indicator",
	-- Federal Republic greatest (max) lifetime expectancy
	(
	select
		lifeexpectancy
	from
		country
	where
		governmentform = 'Federal Republic'
		and lifeexpectancy is not null
	order by
		lifeexpectancy desc
	limit 1) as "Federal republic",
	-- Republic greatest (max) lifetime expectancy
	(
	select
		lifeexpectancy
	from
		country
	where
		governmentform = 'Republic'
		and lifeexpectancy is not null
	order by
		lifeexpectancy desc
	limit 1) as "Republic",
	-- Others greatest (max) lifetime expectancy
	(
	select
		lifeexpectancy
	from
		country
	where
		governmentform != 'Federal Republic'
		and governmentform != 'Republic'
		and lifeexpectancy is not null
	order by
		lifeexpectancy desc
	limit 1) as "Others"
)
-- Combine with the second row (stack rows vertically)
union
-- Calc the second row: LLE
(
select
	'LLE' as "Indicator",
	-- Federal Republic least (min) lifetime expectancy
	(
	select
		lifeexpectancy
	from
		country
	where
		governmentform = 'Federal Republic'
		and lifeexpectancy is not null
	order by
		lifeexpectancy asc
	limit 1) as "Federal republic",
	-- Republic least (min) lifetime expectancy
	(
	select
		lifeexpectancy
	from
		country
	where
		governmentform = 'Republic'
		and lifeexpectancy is not null
	order by
		lifeexpectancy asc
	limit 1) as "Republic",
	-- Others least (min) lifetime expectancy
	(
	select
		lifeexpectancy
	from
		country
	where
		governmentform != 'Federal Republic'
		and governmentform != 'Republic'
		and lifeexpectancy is not null
	order by
		lifeexpectancy asc
	limit 1) as "Others"
);

/* Result:
Indicator|Federal republic|Republic|Others|
---------+----------------+--------+------+
GLE      |            77.7|    81.1|  83.5|
LLE      |            51.6|    37.2|  40.4|
*/
