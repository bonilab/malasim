# Review 
- Note: PersonEvent derived class should not have person_ member, use get_person() instead
- Check why Population initialize takes soo long.
-  RandomTest.random_multinomial_DistributionMatchesMultinomial_ChiSquaredTest sometimes failed????
-  Improve speed for RouletteTest.compare_with_multi_normial

- ImmuneSystem unique pointer
- Person in Population to unique ptr

- p_infection_from_infectious_bite biology validation
- relative_infectivity biology validation

- In config population demographic, why number_of_age_classes have to be set before the vector age_clasess?

- movement_settings: as we normally not change it much due to lack of data?
- Events in Person need to review? x Done
- Need unittest for event queue

# Recrudescence

- A seperate simulation to validate
- add number of recurrence treatments to reporter

# Unittest
- ImmuneSystem

- If there is an old test failed. Try Model::get_instance()->initialize()

# Random class

- [ ] Random unit tests

# Project wide

- [ ] add gcov
- [ ] add CI/Coverage
- [ ] choose a logging library
