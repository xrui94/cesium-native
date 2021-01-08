const path = require("path");
const fs = require("fs");
const lodash = require("lodash");
const resolveProperty = require("./resolveProperty");
const getNameFromSchema = require("./getNameFromSchema");
const unindent = require("./unindent");
const indent = require("./indent");

function generate(options, schema) {
  const { schemaCache, nameMapping, outputDir, readerOutputDir, cesiumProperties } = options;

  const name = getNameFromSchema(nameMapping, schema);

  console.log(`Generating ${name}`);

  let base = "ExtensibleObject";
  if (schema.allOf && schema.allOf.length > 0 && schema.allOf[0].$ref) {
    const baseSchema = schemaCache.load(schema.allOf[0].$ref);
    base = getNameFromSchema(nameMapping, baseSchema);
  }

  const properties = Object.keys(schema.properties)
    .map((key) =>
      resolveProperty(schemaCache, nameMapping, name, key, schema.properties[key])
    )
    .filter((property) => property !== undefined);

  const localTypes = lodash.uniq(
    lodash.flatten(properties.map((property) => property.localTypes))
  );

  const headers = lodash.uniq(
    [`"CesiumGltf/${base}.h"`, ...lodash.flatten(properties.map((property) => property.headers))]
  );

  if (cesiumProperties[name]) {
    headers.push(`"CesiumGltf/${name}Cesium.h"`);
  }

  headers.sort();

  const header = `
        // This file was generated by generate-gltf-classes.
        // DO NOT EDIT THIS FILE!
        #pragma once

        ${headers.map((header) => `#include ${header}`).join("\n")}
        
        namespace CesiumGltf {
            /**
             * @brief ${schema.description}
             */
            struct ${name} : public ${base} {
                ${indent(localTypes.join("\n\n"), 16)}

                ${indent(
                  properties
                    .map((property) => formatProperty(property))
                    .join("\n\n"),
                  16
                )}

                ${indent(getCesiumProperty(cesiumProperties, name, schema), 16)}
            };
        }
    `;

  const headerOutputDir = path.join(outputDir, "include", "CesiumGltf");
  fs.mkdirSync(headerOutputDir, { recursive: true });
  const headerOutputPath = path.join(headerOutputDir, name + ".h");
  fs.writeFileSync(headerOutputPath, unindent(header), "utf-8");

  const readerHeaders = lodash.uniq(
    [`"${base}JsonHandler.h"`, ...lodash.flatten(properties.map((property) => property.readerHeaders))]
  );
  readerHeaders.sort();

  const readerLocalTypes = lodash.uniq(
    lodash.flatten(properties.map((property) => property.readerLocalTypes))
  );

  const readerHeader = `
        // This file was generated by generate-gltf-classes.
        // DO NOT EDIT THIS FILE!
        #pragma once

        ${readerHeaders.map((header) => `#include ${header}`).join("\n")}

        namespace CesiumGltf {
          struct ${name};

          class ${name}JsonHandler : public ${base}JsonHandler {
          public:
            void reset(IJsonHandler* pHandler, ${name}* pObject);
            ${name}* getObject();
            virtual void reportWarning(const std::string& warning, std::vector<std::string>&& context = std::vector<std::string>()) override;

            virtual IJsonHandler* Key(const char* str, size_t length, bool copy) override;

          protected:
            IJsonHandler* ${name}Key(const char* str, ${name}& o);
    
          private:
            ${indent(readerLocalTypes.join("\n\n"), 12)}

            ${name}* _pObject;
            ${indent(
              properties
                .map((property) => formatReaderProperty(property))
                .join("\n"),
              12
            )}
          };
        }
  `;

  const readerHeaderOutputDir = path.join(readerOutputDir, "generated");
  fs.mkdirSync(readerHeaderOutputDir, { recursive: true });
  const readerHeaderOutputPath = path.join(readerHeaderOutputDir, name + "JsonHandler.h");
  fs.writeFileSync(readerHeaderOutputPath, unindent(readerHeader), "utf-8");

  const readerLocalTypesImpl = lodash.uniq(
    lodash.flatten(properties.map((property) => property.readerLocalTypesImpl))
  );

  const readerImpl = `
        // This file was generated by generate-gltf-classes.
        // DO NOT EDIT THIS FILE!
        #include "${name}JsonHandler.h"
        #include "CesiumGltf/${name}.h"
        #include <cassert>
        #include <string>

        using namespace CesiumGltf;

        void ${name}JsonHandler::reset(IJsonHandler* pParent, ${name}* pObject) {
          ${base}JsonHandler::reset(pParent, pObject);
          this->_pObject = pObject;
        }

        ${name}* ${name}JsonHandler::getObject() {
          return this->_pObject;
        }

        void ${name}JsonHandler::reportWarning(const std::string& warning, std::vector<std::string>&& context) {
          if (this->getCurrentKey()) {
            context.emplace_back(std::string(".") + this->getCurrentKey());
          }
          this->parent()->reportWarning(warning, std::move(context));
        }

        IJsonHandler* ${name}JsonHandler::Key(const char* str, size_t /*length*/, bool /*copy*/) {
          assert(this->_pObject);
          return this->${name}Key(str, *this->_pObject);
        }

        IJsonHandler* ${name}JsonHandler::${name}Key(const char* str, ${name}& o) {
          using namespace std::string_literals;

          ${indent(
            properties
              .map((property) => formatReaderPropertyImpl(property))
              .join("\n"),
            10
          )}

          return this->${base}Key(str, *this->_pObject);
        }

        ${indent(readerLocalTypesImpl.join("\n\n"), 8)}
  `;

  const readerSourceOutputPath = path.join(readerHeaderOutputDir, name + "JsonHandler.cpp");
  fs.writeFileSync(readerSourceOutputPath, unindent(readerImpl), "utf-8");

  return lodash.uniq(
    lodash.flatten(properties.map((property) => property.schemas))
  );
}

function formatProperty(property) {
  let result = "";

  result += `/**\n * @brief ${property.briefDoc || property.name}\n`;
  if (property.fullDoc) {
    result += ` *\n * ${property.fullDoc.split("\n").join("\n * ")}\n`;
  }

  result += ` */\n`;

  result += `${property.type} ${property.name};`;

  return result;
}

function formatReaderProperty(property) {
  return `${property.readerType} _${property.name};`
}

function formatReaderPropertyImpl(property) {
  return `if ("${property.name}"s == str) return property("${property.name}", this->_${property.name}, o.${property.name});`;
}

function getCesiumProperty(cesiumProperties, name, schema) {
  if (!cesiumProperties[name]) {
    return "";
  }

  return unindent(`
    /**
     * @brief Holds properties that are specific to the glTF loader rather than part of the glTF spec.
     */
    ${name}Cesium cesium;
  `);
}

module.exports = generate;
