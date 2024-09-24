# Project Folder Structure

## Overview

This document outlines the basic folder structure for the project.

## Folder Structure

```
src/
├── Core/
│   ├── Config/
│   ├── Random/
│   ├── Scheduler/
│   └── Model.h
│   └── Model.cpp
├── Population/
│   ├── Person/
│   ├── ImmuneSystem/
│   ├── ParasiteDensity/
│   └── Population.h
│   └── Population.cpp
├── Events/
│   ├── Population/
│   ├── Environment/
│   └── EventManager.h
│   └── EventManager.cpp
├── Spatial/
│   ├── Location/
│   ├── Movement/
│   └── SpatialModel.h
│   └── SpatialModel.cpp
├── Treatment/
│   ├── Strategies/
│   ├── Therapies/
│   └── TreatmentManager.h
│   └── TreatmentManager.cpp
├── Reporters/
│   ├── Specialist/
│   ├── Utility/
│   └── ReportManager.h
│   └── ReportManager.cpp
├── Parasites/
│   └── ParasiteManager.h
│   └── ParasiteManager.cpp
├── Environment/
│   └── EnvironmentManager.h
│   └── EnvironmentManager.cpp
└── Simulation/
    └── Simulation.h
    └── Simulation.cpp
