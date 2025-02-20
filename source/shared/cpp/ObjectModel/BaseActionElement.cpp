// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
#include "pch.h"
#include "BaseActionElement.h"
#include "BaseElement.h"
#include "ParseUtil.h"

using namespace AdaptiveSharedNamespace;

constexpr const char* const BaseActionElement::defaultStyle;

BaseActionElement::BaseActionElement(ActionType type) : m_style(BaseActionElement::defaultStyle), m_type(type), m_mode(Mode::Primary)
{
    SetTypeString(ActionTypeToString(type));
    PopulateKnownPropertiesSet();
}

const std::string& BaseActionElement::GetTitle() const
{
    return m_title;
}

void BaseActionElement::SetTitle(std::string&& value)
{
    m_title = std::move(value);
}

void BaseActionElement::SetTitle(const std::string& value)
{
    m_title = value;
}

const std::string& BaseActionElement::GetIconUrl() const
{
    return m_iconUrl;
}

void BaseActionElement::SetIconUrl(std::string&& value)
{
    m_iconUrl = std::move(value);
}

void BaseActionElement::SetIconUrl(const std::string& value)
{
    m_iconUrl = value;
}

const std::string& BaseActionElement::GetStyle() const
{
    return m_style;
}

void BaseActionElement::SetStyle(std::string&& value)
{
    m_style = std::move(value);
}

void BaseActionElement::SetStyle(const std::string& value)
{
    m_style = value;
}

void BaseActionElement::SetMode(const Mode value)
{
    m_mode = value;
}

ActionType BaseActionElement::GetElementType() const
{
    return m_type;
}

Mode BaseActionElement::GetMode() const
{
    return m_mode;
}

Json::Value BaseActionElement::SerializeToJsonValue() const
{
    Json::Value root = BaseElement::SerializeToJsonValue();

    if (!m_iconUrl.empty())
    {
        root[AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::IconUrl)] = m_iconUrl;
    }

    if (!m_title.empty())
    {
        root[AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::Title)] = m_title;
    }

    if (!m_style.empty() && (m_style.compare(defaultStyle) != 0))
    {
        root[AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::Style)] = m_style;
    }
    if (m_mode != Mode::Primary)
    {
        root[AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::Mode)] = ModeToString(m_mode);
    }

    return root;
}

void BaseActionElement::PopulateKnownPropertiesSet()
{
    m_knownProperties.insert({AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::IconUrl),
                              AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::Style),
                              AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::Title),
                              AdaptiveCardSchemaKeyToString(AdaptiveCardSchemaKey::Mode)});
}

void BaseActionElement::GetResourceInformation(std::vector<RemoteResourceInformation>& resourceInfo)
{
    if (!m_iconUrl.empty())
    {
        RemoteResourceInformation imageResourceInfo;
        imageResourceInfo.url = m_iconUrl;
        imageResourceInfo.mimeType = "image";
        resourceInfo.push_back(imageResourceInfo);
    }
}

void BaseActionElement::ParseJsonObject(AdaptiveSharedNamespace::ParseContext& context,
                                        const Json::Value& json,
                                        std::shared_ptr<BaseElement>& baseElement)
{
    baseElement = ParseUtil::GetActionFromJsonValue(context, json);
}

std::shared_ptr<BaseActionElement> BaseActionElement::DeserializeBasePropertiesFromString(ParseContext& context, const std::string& jsonString)
{
    return BaseActionElement::DeserializeBaseProperties(context, ParseUtil::GetJsonValueFromString(jsonString));
}

std::shared_ptr<BaseActionElement> BaseActionElement::DeserializeBaseProperties(ParseContext& context, const Json::Value& json)
{
    std::shared_ptr<BaseActionElement> baseActionElement = std::make_shared<BaseActionElement>();
    DeserializeBaseProperties(context, json, baseActionElement);
    return baseActionElement;
}

void BaseActionElement::DeserializeBaseProperties(ParseContext& context, const Json::Value& json, std::shared_ptr<BaseActionElement>& element)
{
    ParseUtil::ThrowIfNotJsonObject(json);

    element->DeserializeBase<BaseActionElement>(context, json);
    element->SetTitle(ParseUtil::GetString(json, AdaptiveCardSchemaKey::Title));
    element->SetIconUrl(ParseUtil::GetString(json, AdaptiveCardSchemaKey::IconUrl));
    element->SetStyle(ParseUtil::GetString(json, AdaptiveCardSchemaKey::Style, defaultStyle, false));
    element->SetMode(ParseUtil::GetEnumValue<Mode>(json, AdaptiveCardSchemaKey::Mode, Mode::Primary,
                                                   ModeFromString));
}
