#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CSVParser.h"

#define MAX_LOCATIONS 300
#define MAX_NAME 128
#define MAX_DATE 11

typedef struct {
    char location[MAX_NAME];
    char date[MAX_DATE];
    long double total_cases;
    long double total_deaths;
    int seen;
} CountryStats;

typedef struct {
    int continent_idx;
    int location_idx;
    int date_idx;
    int cases_idx;
    int deaths_idx;
    int header_read;
    CountryStats countries[MAX_LOCATIONS];
    int country_count;
} ProcessingContext;

static int find_column_index(char **fields, int field_count, const char *name) {
    for (int i = 0; i < field_count; ++i) {
        if (fields[i] != NULL && strcmp(fields[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

static void copy_string(char *dest, size_t size, const char *src) {
    if (src == NULL) {
        src = "";
    }

    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

static int is_newer_date(const char *a, const char *b) {
    return strcmp(a, b) > 0;
}

static void process_row(char **fields, int field_count, void *userData) {
    ProcessingContext *ctx = (ProcessingContext *)userData;

    if (!ctx->header_read) {
        ctx->continent_idx = find_column_index(fields, field_count, "continent");
        ctx->location_idx = find_column_index(fields, field_count, "location");
        ctx->date_idx = find_column_index(fields, field_count, "date");
        ctx->cases_idx = find_column_index(fields, field_count, "total_cases");
        ctx->deaths_idx = find_column_index(fields, field_count, "total_deaths");
        ctx->header_read = 1;
        return;
    }

    if (ctx->continent_idx < 0 || ctx->location_idx < 0 || ctx->date_idx < 0 ||
        ctx->cases_idx < 0 || ctx->deaths_idx < 0) {
        return;
    }

    if (ctx->continent_idx >= field_count || ctx->location_idx >= field_count ||
        ctx->date_idx >= field_count || ctx->cases_idx >= field_count ||
        ctx->deaths_idx >= field_count) {
        return;
    }

    if (strcmp(fields[ctx->continent_idx], "South America") != 0) {
        return;
    }

    const char *location = fields[ctx->location_idx];
    const char *date = fields[ctx->date_idx];
    const char *cases_str = fields[ctx->cases_idx];
    const char *deaths_str = fields[ctx->deaths_idx];

    int found = 0;
    for (int i = 0; i < ctx->country_count; ++i) {
        if (strcmp(ctx->countries[i].location, location) == 0) {
            found = 1;
            if (!ctx->countries[i].seen || is_newer_date(date, ctx->countries[i].date)) {
                copy_string(ctx->countries[i].date, sizeof(ctx->countries[i].date), date);
                ctx->countries[i].total_cases = cases_str[0] ? (long double)strtod(cases_str, NULL) : 0.0L;
                ctx->countries[i].total_deaths = deaths_str[0] ? (long double)strtod(deaths_str, NULL) : 0.0L;
                ctx->countries[i].seen = 1;
            }
            break;
        }
    }

    if (!found) {
        if (ctx->country_count >= MAX_LOCATIONS) {
            return;
        }

        copy_string(ctx->countries[ctx->country_count].location, sizeof(ctx->countries[ctx->country_count].location), location);
        copy_string(ctx->countries[ctx->country_count].date, sizeof(ctx->countries[ctx->country_count].date), date);
        ctx->countries[ctx->country_count].total_cases = cases_str[0] ? (long double)strtod(cases_str, NULL) : 0.0L;
        ctx->countries[ctx->country_count].total_deaths = deaths_str[0] ? (long double)strtod(deaths_str, NULL) : 0.0L;
        ctx->countries[ctx->country_count].seen = 1;
        ctx->country_count++;
    }
}

int main(int argc, char **argv) {
    const char *csv_path = (argc > 1) ? argv[1] : "owid-covid-data.csv";
    FILE *fp = fopen(csv_path, "rb");

    if (!fp) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo: %s\n", csv_path);
        return 1;
    }

    ProcessingContext ctx;
    memset(&ctx, 0, sizeof(ctx));

    CSVParser parser;
    CSVParser_init(&parser);

    char buffer[8192];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        CSVParser_processLines(&parser, buffer, (int)bytes_read, process_row, &ctx);
    }

    fclose(fp);
    CSVParser_processLines(&parser, "\n", 1, process_row, &ctx);

    if (!ctx.header_read) {
        fprintf(stderr, "Arquivo vazio ou sem cabecalho.\n");
        return 1;
    }

    if (ctx.continent_idx < 0 || ctx.location_idx < 0 || ctx.date_idx < 0 ||
        ctx.cases_idx < 0 || ctx.deaths_idx < 0) {
        fprintf(stderr, "Colunas esperadas nao foram encontradas no CSV.\n");
        return 1;
    }

    long double total_cases = 0.0L;
    long double total_deaths = 0.0L;

    for (int i = 0; i < ctx.country_count; ++i) {
        total_cases += ctx.countries[i].total_cases;
        total_deaths += ctx.countries[i].total_deaths;
    }

    printf("Total de casos na America do Sul: %.0Lf\n", total_cases);
    printf("Total de mortes na America do Sul: %.0Lf\n", total_deaths);

    return 0;
}
