analysis <- function(file_name)
{
  data <- read.csv(file_name, header <- T, sep <- ',')
  values <- data[['minimum']]
  cat(sd(values), "\n")
  cat(mean(data[['time_measurement']]))
}

# future idea make a function for plots 
analysis("test_output.csv")