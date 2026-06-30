#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELDS 200
#define MAX_LINE 20000
#define MAX_LOCATIONS 300
#define MAX_NAME 128

typedef struct {
    char location[MAX_NAME];
    char date[11];
    long double total_cases;
    long double total_deaths;
    int seen;
} CountryStats;

static int split_csv_line(char *line, char **fields, int max_fields) {
    int count = 0;
    int in_quotes = 0;
    char *start = line;

    fields[count++] = start;

    for (char *p = line; *p != '\0'; ++p) {
        if (*p == '"') {
            in_quotes = !in_quotes;
        } else if (*p == ',' && !in_quotes) {
            *p = '\0';
            if (count < max_fields) {
                fields[count++] = p + 1;
            }
        }
    }

    return count;
}

static int find_column_index(char **fields, int field_count, const char *name) {
    for (int i = 0; i < field_count; ++i) {
        if (strcmp(fields[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

static int is_newer_date(const char *a, const char *b) {
    return strcmp(a, b) > 0;
}

int main(int argc, char **argv) {
    const char *csv_path = (argc > 1) ? argv[1] : "owid-covid-data.csv";
    FILE *fp = fopen(csv_path, "r");

    if (!fp) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo: %s\n", csv_path);
        return 1;
    }

    char line[MAX_LINE];
    if (!fgets(line, sizeof(line), fp)) {
        fprintf(stderr, "Arquivo vazio.\n");
        fclose(fp);
        return 1;
    }

    char *header_fields[MAX_FIELDS];
    int header_count = split_csv_line(line, header_fields, MAX_FIELDS);

    int continent_idx = find_column_index(header_fields, header_count, "continent");
    int location_idx = find_column_index(header_fields, header_count, "location");
    int date_idx = find_column_index(header_fields, header_count, "date");
    int cases_idx = find_column_index(header_fields, header_count, "total_cases");
    int deaths_idx = find_column_index(header_fields, header_count, "total_deaths");

    if (continent_idx < 0 || location_idx < 0 || date_idx < 0 || cases_idx < 0 || deaths_idx < 0) {
        fprintf(stderr, "Colunas esperadas nao foram encontradas no CSV.\n");
        fclose(fp);
        return 1;
    }

    CountryStats countries[MAX_LOCATIONS];
    int country_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *fields[MAX_FIELDS];
        int field_count = split_csv_line(line, fields, MAX_FIELDS);

        if (field_count <= 0) {
            continue;
        }

        if (continent_idx >= field_count || location_idx >= field_count || date_idx >= field_count ||
            cases_idx >= field_count || deaths_idx >= field_count) {
            continue;
        }

        if (strcmp(fields[continent_idx], "South America") != 0) {
            continue;
        }

        const char *location = fields[location_idx];
        const char *date = fields[date_idx];

        int found = 0;
        for (int i = 0; i < country_count; ++i) {
            if (strcmp(countries[i].location, location) == 0) {
                found = 1;
                if (!countries[i].seen || is_newer_date(date, countries[i].date)) {
                    strncpy(countries[i].date, date, sizeof(countries[i].date) - 1);
                    countries[i].date[sizeof(countries[i].date) - 1] = '\0';
                    countries[i].total_cases = fields[cases_idx][0] ? (long double)strtod(fields[cases_idx], NULL) : 0.0L;
                    countries[i].total_deaths = fields[deaths_idx][0] ? (long double)strtod(fields[deaths_idx], NULL) : 0.0L;
                    countries[i].seen = 1;
                }
                break;
            }
        }

        if (!found) {
            if (country_count >= MAX_LOCATIONS) {
                break;
            }
            strncpy(countries[country_count].location, location, MAX_NAME - 1);
            countries[country_count].location[MAX_NAME - 1] = '\0';
            strncpy(countries[country_count].date, date, sizeof(countries[country_count].date) - 1);
            countries[country_count].date[sizeof(countries[country_count].date) - 1] = '\0';
            countries[country_count].total_cases = fields[cases_idx][0] ? (long double)strtod(fields[cases_idx], NULL) : 0.0L;
            countries[country_count].total_deaths = fields[deaths_idx][0] ? (long double)strtod(fields[deaths_idx], NULL) : 0.0L;
            countries[country_count].seen = 1;
            country_count++;
        }
    }

    long double total_cases = 0.0L;
    long double total_deaths = 0.0L;

    for (int i = 0; i < country_count; ++i) {
        total_cases += countries[i].total_cases;
        total_deaths += countries[i].total_deaths;
    }

    printf("Total de casos na America do Sul: %.0Lf\n", total_cases);
    printf("Total de mortes na America do Sul: %.0Lf\n", total_deaths);

    fclose(fp);
    return 0;
}
