
SELECT employees.name, departments.dept_name FROM employees JOIN departments ON employees.dept_id = departments.dept_id WHERE employees.salary = 0;
SELECT employees.name, departments.dept_name FROM employees JOIN departments ON employees.dept_id = departments.dept_id WHERE departments.dept_name = 'Engineering';
SELECT employees.name, salaries.salary FROM employees JOIN salaries ON employees.emp_id = salaries.emp_id WHERE salaries.salary > 50000;