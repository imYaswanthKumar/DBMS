# Relational Algebra

## Overview

This project implements a basic relational algebra system in C++ that supports various operations on CSV-based relations. The system allows you to perform operations like selection, projection, renaming, and more, using a query language with a specific syntax.

## Features

| **Operation**         | **Syntax**                                                     | **Description**                                                  |
|-----------------------|----------------------------------------------------------------|------------------------------------------------------------------|
| **Select**            | `S[predicate](relation)`                                       | Filters rows based on a predicate. Only constants (strings or integers) should be enclosed in single quotes. |
| **Project**           | `P[column list](relation)`                                     | Selects specific columns from a relation. Use `*` to select all columns. Columns in the list are separated by commas. |
| **Rename**            | `R[new relation name(new column names)](old relation name)`    | Renames a relation and/or its columns. You can either rename all columns or keep the existing names. |
| **Union**             | `U[relation_1](relation_2)`                                    | Computes the union of two relations. |
| **Intersection**      | `I[relation_1](relation_2)`                                    | Computes the intersection of two relations. |
| **Set Difference**    | `D[relation_1](relation_2)`                                    | Computes the difference between two relations (relation_1 - relation_2). |
| **Cartesian Product** | `C[relation_1](relation_2)`                                    | Computes the Cartesian product of two relations. |
| **Exit**              | `exit`                                                         | Terminates the query session. |

## Notes

- In the `Select` operation, constants (strings or integers) should be enclosed in single quotes.
- The table name is derived from the CSV file name, and only columns in those files can be used.
- Queries are case-insensitive.
- When renaming, you must provide names for all columns or none at all. The relation can be renamed independently.

## Usage

1. **Load CSV Files:** Ensure your CSV files are named appropriately as they determine the table names in the system.
2. **Start the Query Session**: Run the application to begin the query session.
3. **Execute Queries:** Use the syntax provided to perform operations on your relations.
4. **Exit**: Type `exit` to terminate the query session.
