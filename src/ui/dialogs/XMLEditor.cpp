/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#include "XMLEditor.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFontDatabase>
#include <QSyntaxHighlighter>
#include <QTextBlock>
#include <QTextCursor>

#include <QDomDocument>

namespace QVirt {

/**
 * Simple XML Syntax Highlighter
 */
class XMLSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    XMLSyntaxHighlighter(QTextDocument *parent = nullptr)
        : QSyntaxHighlighter(parent) {}

protected:
    void highlightBlock(const QString &text) override
    {

        QTextCharFormat keywordFormat;
        keywordFormat.setForeground(QColor(0, 0, 255));
        keywordFormat.setFontWeight(QFont::Bold);

        QTextCharFormat tagFormat;
        tagFormat.setForeground(QColor(163, 21, 21));

        QTextCharFormat attributeFormat;
        attributeFormat.setForeground(QColor(106, 135, 89));

        QTextCharFormat valueFormat;
        valueFormat.setForeground(QColor(204, 123, 52));

        QTextCharFormat commentFormat;
        commentFormat.setForeground(QColor(128, 128, 128));
        commentFormat.setFontItalic(true);

        int state = 0;
        int i = 0;

        while (i < text.length()) {
            QChar ch = text.at(i);

            if (ch == '<') {
                // Tag start
                if (i + 1 < text.length() && text.at(i + 1) == '/') {
                    // Closing tag
                    setFormat(i, 2, tagFormat);
                    i += 2;
                } else if (i + 3 < text.length() && text.mid(i, 4) == "<!--") {
                    // Comment
                    int end = text.indexOf("-->", i);
                    if (end > 0) {
                        setFormat(i, end - i + 3, commentFormat);
                        i = end + 3;
                    } else {
                        i++;
                    }
                } else {
                    // Opening tag
                    int end = text.indexOf('>', i);
                    if (end > 0) {
                        setFormat(i, end - i + 1, tagFormat);
                        i = end + 1;
                    } else {
                        i++;
                    }
                }
                state = 1;
            } else if (ch == '>' && state == 1) {
                state = 0;
                i++;
            } else if (ch == '"' && state == 1) {
                // Attribute value
                int end = text.indexOf('"', i + 1);
                if (end > 0) {
                    setFormat(i, end - i + 1, valueFormat);
                    i = end + 1;
                } else {
                    i++;
                }
            } else if (ch.isLetter() && state == 1) {
                // Attribute name
                int end = i;
                while (end < text.length() && (text.at(end).isLetterOrNumber() || text.at(end) == '_')) {
                    end++;
                }
                if (end < text.length() && text.at(end) == '=') {
                    setFormat(i, end - i, attributeFormat);
                }
                i = end;
            } else {
                i++;
            }
        }

        setCurrentBlockState(0);
    }
};

//=============================================================================
// XMLEditor
//=============================================================================

XMLEditor::XMLEditor(QWidget *parent)
    : QDialog(parent)
    , m_validateXML(true)
{
    setWindowTitle("XML Editor");
    resize(800, 600);

    setupUI();
}

void XMLEditor::setupUI()
{
    auto *layout = new QVBoxLayout(this);

    // Description label
    m_descriptionLabel = new QLabel(this);
    m_descriptionLabel->setWordWrap(true);
    layout->addWidget(m_descriptionLabel);

    // Editor
    m_editor = new QPlainTextEdit(this);
    m_editor->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    m_editor->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_editor->setTabChangesFocus(true);
    m_editor->setTabStopWidth(4);

    // Set monospace font and basic styling
    QFont font = m_editor->font();
    font.setFamily("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    m_editor->setFont(font);

    // Add syntax highlighting
    new XMLSyntaxHighlighter(m_editor->document());

    layout->addWidget(m_editor);

    // Status bar
    auto *statusLayout = new QHBoxLayout();

    m_statusLabel = new QLabel("Ready", this);
    statusLayout->addWidget(m_statusLabel);
    statusLayout->addStretch();

    layout->addLayout(statusLayout);

    // Control buttons
    auto *controlLayout = new QHBoxLayout();

    m_btnValidate = new QPushButton("Validate", this);
    m_btnFormat = new QPushButton("Format", this);
    m_btnReset = new QPushButton("Reset", this);

    controlLayout->addWidget(m_btnValidate);
    controlLayout->addWidget(m_btnFormat);
    controlLayout->addStretch();

    layout->addLayout(controlLayout);

    // OK/Cancel buttons
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_btnOK = new QPushButton("OK", this);
    m_btnCancel = new QPushButton("Cancel", this);

    buttonLayout->addWidget(m_btnOK);
    buttonLayout->addWidget(m_btnCancel);

    layout->addLayout(buttonLayout);

    // Connect signals
    connect(m_editor, &QPlainTextEdit::textChanged,
            this, &XMLEditor::onTextChanged);
    connect(m_btnValidate, &QPushButton::clicked,
            this, &XMLEditor::validate);
    connect(m_btnFormat, &QPushButton::clicked,
            this, &XMLEditor::onFormatXML);
    connect(m_btnReset, &QPushButton::clicked,
            this, &XMLEditor::onReset);
    connect(m_btnOK, &QPushButton::clicked,
            this, &QDialog::accept);
    connect(m_btnCancel, &QPushButton::clicked,
            this, &QDialog::reject);

    // Initial state
    onTextChanged();
}

void XMLEditor::setXML(const QString &xml)
{
    m_originalXML = xml;
    m_editor->setPlainText(xml);
    onTextChanged();
}

QString XMLEditor::getXML() const
{
    return m_editor->toPlainText();
}

void XMLEditor::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void XMLEditor::setDescription(const QString &description)
{
    m_descriptionLabel->setText(description);
}

void XMLEditor::setReadOnly(bool readOnly)
{
    m_editor->setReadOnly(readOnly);
    m_btnOK->setEnabled(!readOnly);
}

void XMLEditor::setValidateXML(bool validate)
{
    m_validateXML = validate;
}

void XMLEditor::onTextChanged()
{
    bool hasChanges = (m_editor->toPlainText() != m_originalXML);
    m_btnReset->setEnabled(hasChanges);

    if (m_validateXML) {
        validate();
    }
}

void XMLEditor::validate()
{
    if (!m_validateXML) {
        m_statusLabel->setText("Validation disabled");
        m_statusLabel->setStyleSheet("");
        return;
    }

    QString xml = m_editor->toPlainText();
    if (xml.isEmpty()) {
        m_statusLabel->setText("Empty XML");
        m_statusLabel->setStyleSheet("color: orange;");
        return;
    }

    if (isValidXML(xml)) {
        m_statusLabel->setText("Valid XML");
        m_statusLabel->setStyleSheet("color: green;");
        m_btnOK->setEnabled(true);
    } else {
        m_statusLabel->setText("Invalid XML");
        m_statusLabel->setStyleSheet("color: red;");
        m_btnOK->setEnabled(false);
    }
}

bool XMLEditor::isValidXML(const QString &xml)
{
    QDomDocument doc;
    QString errorMsg;
    int errorLine, errorCol;

    if (!doc.setContent(xml, &errorMsg, &errorLine, &errorCol)) {
        return false;
    }
    return true;
}

void XMLEditor::onFormatXML()
{
    QString xml = m_editor->toPlainText();

    QDomDocument doc;
    if (!doc.setContent(xml)) {
        QMessageBox::warning(this, "Format Failed",
            "Cannot format invalid XML. Please fix errors first.");
        return;
    }

    // Format with proper indentation
    QString formatted = doc.toString(4);

    m_editor->setPlainText(formatted);
    m_statusLabel->setText("XML Formatted");
}

void XMLEditor::onReset()
{
    int result = QMessageBox::question(this, "Reset XML",
        "Are you sure you want to reset to the original XML?",
        QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        m_editor->setPlainText(m_originalXML);
    }
}

//=============================================================================
// VMXMLEditor
//=============================================================================

VMXMLEditor::VMXMLEditor(const QString &vmName, QWidget *parent)
    : XMLEditor(parent)
    , m_vmName(vmName)
{
    setTitle(QString("VM XML Editor - %1").arg(vmName));
    setDescription(
        "Editing the XML configuration directly can damage the VM. "
        "Only modify this if you know what you're doing."
    );
}

void VMXMLEditor::setDomainXML(const QString &xml, virDomainPtr domain)
{
    Q_UNUSED(domain);
    setXML(xml);
}

QString VMXMLEditor::getDomainXML() const
{
    return getXML();
}

} // namespace QVirt
