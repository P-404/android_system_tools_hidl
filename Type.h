/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TYPE_H_

#define TYPE_H_

#include <android-base/macros.h>
#include <string>
#include <utils/Errors.h>

namespace android {

struct Formatter;
struct ScalarType;

struct Type {
    Type();
    virtual ~Type();

    Type *ref() { return this; }

    virtual bool isScope() const;
    virtual bool isInterface() const;
    virtual bool isEnum() const;
    virtual bool isTypeDef() const;
    virtual bool isBinder() const;
    virtual bool isNamedType() const;

    virtual const ScalarType *resolveToScalarType() const;

    bool isValidEnumStorageType() const;

    enum StorageMode {
        StorageMode_Stack,
        StorageMode_Argument,
        StorageMode_Result
    };
    virtual std::string getCppType(
            StorageMode mode, std::string *extra) const;

    // Convenience, gets StorageMode_Stack type.
    std::string getCppType(std::string *extra) const;

    std::string getCppResultType(std::string *extra) const;
    std::string getCppArgumentType(std::string *extra) const;

    virtual std::string getJavaType() const = 0;
    virtual std::string getJavaWrapperType() const;
    virtual std::string getJavaSuffix() const;

    enum ErrorMode {
        ErrorMode_Ignore,
        ErrorMode_Goto,
        ErrorMode_Break,
        ErrorMode_Return,
    };
    virtual void emitReaderWriter(
            Formatter &out,
            const std::string &name,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode) const;

    virtual void emitReaderWriterEmbedded(
            Formatter &out,
            const std::string &name,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode,
            const std::string &parentName,
            const std::string &offsetText) const;

    virtual void emitJavaReaderWriter(
            Formatter &out,
            const std::string &parcelObj,
            const std::string &argName,
            bool isReader) const;

    virtual void emitJavaFieldInitializer(
            Formatter &out,
            const std::string &fieldName) const;

    virtual void emitJavaFieldReaderWriter(
            Formatter &out,
            const std::string &blobName,
            const std::string &fieldName,
            const std::string &offset,
            bool isReader) const;

    virtual status_t emitTypeDeclarations(Formatter &out) const;

    virtual status_t emitTypeDefinitions(
            Formatter &out, const std::string prefix) const;

    virtual status_t emitJavaTypeDeclarations(
            Formatter &out, bool atTopLevel) const;

    virtual bool needsEmbeddedReadWrite() const;
    virtual bool resultNeedsDeref() const;

    // Generates type declaration for vts proto file.
    // TODO (b/30844146): make it a pure virtual method.
    virtual status_t emitVtsTypeDeclarations(Formatter &out) const;
    // Generates type declaration as attribute of method (return value or method
    // argument) or attribute of compound type for vts proto file.
    virtual status_t emitVtsAttributeType(Formatter &out) const;

    // Returns true iff this type is supported through the Java backend.
    virtual bool isJavaCompatible() const;

    virtual void getAlignmentAndSize(size_t *align, size_t *size) const;

protected:
    void handleError(Formatter &out, ErrorMode mode) const;
    void handleError2(Formatter &out, ErrorMode mode) const;

    void emitReaderWriterEmbeddedForTypeName(
            Formatter &out,
            const std::string &name,
            bool nameIsPointer,
            const std::string &parcelObj,
            bool parcelObjIsPointer,
            bool isReader,
            ErrorMode mode,
            const std::string &parentName,
            const std::string &offsetText,
            const std::string &typeName,
            const std::string &childName) const;

    void emitJavaReaderWriterWithSuffix(
            Formatter &out,
            const std::string &parcelObj,
            const std::string &argName,
            bool isReader,
            const std::string &suffix,
            const std::string &extra) const;

private:
    DISALLOW_COPY_AND_ASSIGN(Type);
};

}  // namespace android

#endif  // TYPE_H_

