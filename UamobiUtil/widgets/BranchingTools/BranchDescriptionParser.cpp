#include "BranchDescriptionParser.h"




QHash<QString, independent_nodes::nodelist> _initBinding()
{
	using namespace independent_nodes;
	QHash<QString, independent_nodes::nodelist> h;
	h["PagedSearch"] = PagedSearch;
	h["FilterSelect"] = FilterSelect;
	h["DocResults"] = DocResults;
	h["EditableDocResults"] = EditableDocResults;
	h["SubbranchingDocResults"] = SubbranchingDocResults;
	h["ReceiptParameters"] = ReceiptParameters;
	h["NormalScaning"] = NormalScaning;
	h["IdDependentSelect"] = IdDependentSelect;
	h["SelectItemFromList"] = SelectItemFromList;
	h["PlaceSelect"] = PlaceSelect;
	h["InventoryParameters"] = InventoryParameters;
	h["ParentDocument"] = ParentDocument;
	h["BarcodeFilterSelect"] = BarcodeFilterSelect;
	h["Subbranch"] = Subbranch;
	h["PrintingScaning"] = PrintingScaning;
	h["Switch"] = Switch;
	h["Sender"] = Sender;
	h["MulticontrolScaning"] = MulticontrolScaning;
	h["StaticSubbranch"] = StaticSubbranch;
	h["ObservedScaning"] = ObservedScaning;
	h["ListPickObserver"] = ListPickObs;
	h["Skip"] = SkipNode;
	h["Default"] = DefaultNode;
	h["SwitchByScanCode"] = SwitchByScanBc;
	h["IdDependMultiselect"] = IdDependentMultiselect;
	h["NormalScaningWithPrint"] = NormalScaningWithPrint;
	h["SelectItemByBarcode"] = SelectItemByBarcode;
	h["IdDependentSelectByBarcode"] = IdDependentSelectByBarcode;
	return h;
}

static QHash<QString, independent_nodes::nodelist> widgetBinding(_initBinding());

QHash<QString, UniformXmlObject::ThingsIds> bindEntities()
{
	QHash<QString, UniformXmlObject::ThingsIds> h;
	h["Mode"] = UniformXmlObject::Mode;
	h["Place"] = UniformXmlObject::Place;
	h["Supplier"] = UniformXmlObject::Supplier;
	h["Order"] = UniformXmlObject::Order;
	h["Item"] = UniformXmlObject::Item;
	h["SimpleItem"] = UniformXmlObject::SimpleItem;
	h["Document"] = UniformXmlObject::Document;
	h["Doctype"] = UniformXmlObject::Doctype;
	h["Group"] = UniformXmlObject::Group;
	h["Stillage"] = UniformXmlObject::Stillage;
	h["Page"] = UniformXmlObject::Page;
	h["Control"] = UniformXmlObject::Control;
	h["User"] = UniformXmlObject::User;
	h["StatusString"] = UniformXmlObject::StatusString;
	h["LesserDocument"] = UniformXmlObject::LesserDocument;
	h["Richtext"] = UniformXmlObject::Richtext;
	h["Control"] = UniformXmlObject::Control;
	h["Barcode"] = UniformXmlObject::Barcode;
	return h;
}
static QHash<QString, UniformXmlObject::ThingsIds> entityBinding(bindEntities());


void BranchDescriptionParser::_prepareString(QString& buffer)
{
	buffer = buffer.trimmed();
	buffer.chop(1);
	buffer = buffer.right(buffer.count() - 1);
}

void BranchDescriptionParser::_extractAndPush(QString buffer)
{
	_prepareString(buffer);
	if (currentToAdd->type != independent_nodes::NotANode)
	{
		currentNode->emplaceNode(currentToAdd);
	}
	currentToAdd->type = widgetBinding.value(buffer);
}

void BranchDescriptionParser::_extractEntity(QString buffer)
{
	_prepareString(buffer);
	switch (entityBinding.value(buffer))
	{
	case UniformXmlObject::Mode:
        currentToAdd->entity = RecEntity(new ModeEntity());
			break;
	case UniformXmlObject::Place:
        currentToAdd->entity = RecEntity(new PlaceEntity());
			break;
	case UniformXmlObject::Supplier:
        currentToAdd->entity = RecEntity(new SupplierEntity());
			break;
	case UniformXmlObject::Order:
        currentToAdd->entity = RecEntity(new OrderEntity());
			break;
	case UniformXmlObject::Item:
        currentToAdd->entity = RecEntity(new FullItemEntity());
			break;
	case UniformXmlObject::SimpleItem:
        currentToAdd->entity = RecEntity(new ShortItemEntity());
			break;
	case UniformXmlObject::Document:
        currentToAdd->entity = RecEntity(new FullDocumentEntity());
			break;
	case UniformXmlObject::Doctype:
        currentToAdd->entity = RecEntity(new DocTypeEntity());
			break;
	case UniformXmlObject::Group:
        currentToAdd->entity = RecEntity(new GroupEntity());
			break;
	case UniformXmlObject::Stillage:
        currentToAdd->entity = RecEntity(new StillageEntity());
			break;
	case UniformXmlObject::Control:
        currentToAdd->entity = RecEntity(new InputControlEntity());
			break;
	case UniformXmlObject::User:
        currentToAdd->entity = RecEntity(new UserEntity());
			break;
	case UniformXmlObject::LesserDocument:
        currentToAdd->entity = RecEntity(new LesserDocumentEntity());
			break;
	default:
		return;
	}
}

void BranchDescriptionParser::_extractOverload(QString buffer)
{
	using namespace QueryTemplates;
	_prepareString(buffer);
	if (buffer.isEmpty())
	{
		currentToAdd->oqs.push_back(OverloadableQuery::defaultQuery());
		return;
	}
	if (buffer.count() == 1 && buffer.startsWith(':'))
	{
		currentToAdd->oqs.push_back(OverloadableQuery(OverloadableQuery::NullQ, ping));
		return;
	}
    QStringList temp = buffer.split(" : ");
	if (temp.count() != 2)
	{
		if (temp.count() == 1)
            currentToAdd->oqs.push_back(OverloadableQuery(temp.at(0)));
		return;
	}
	
    QStringList args(temp.at(1).split(" , "));
	currentToAdd->oqs.push_back(OverloadableQuery(temp.first(), args));
}

void BranchDescriptionParser::_extractBacktrack(QString buffer)
{
	_prepareString(buffer);
	bool ok;
	int backtrack = buffer.toInt(&ok);
	if (!ok)
		return;
	currentToAdd->backtracking = backtrack;
}

void BranchDescriptionParser::_openSubbranchCapture()
{
	/*if (currentNode->isEmpty())
		return;*/
	if (currentToAdd->type != independent_nodes::NotANode)
	{
		currentNode->emplaceNode(currentToAdd);
	}
	parentStack.push(currentNode);
	currentNode = currentNode->last().data();
}

void BranchDescriptionParser::_closeSubbranchCapture()
{
	if (parentStack.isEmpty())
		return;
	if (currentToAdd->type != independent_nodes::NotANode)
	{
		currentNode->emplaceNode(currentToAdd);
	}
	currentNode = parentStack.pop();
}

void BranchDescriptionParser::_extractNameOverload(QString buffer)
{
	_prepareString(buffer);
	currentNode->namesOverload << buffer;
}

BranchDescriptionParser::BranchDescriptionParser()
	: cin(), root(), currentNode(), currentToAdd()
{
}

BranchDescription BranchDescriptionParser::doParsing(QString what)
{
    root = BranchDescription(new BranchElementDescription());
    currentToAdd = BranchDescription(new BranchElementDescription());
	currentNode = root.data();
	parentStack.clear();
	cin.setString(&what, QIODevice::ReadOnly);
	QString buffer;
	while (true)
	{
        buffer = cin.readLine();
		if (!buffer.isEmpty())
		{
			switch (buffer.at(0).toLatin1())
			{
			case '[':
				_extractAndPush(buffer);
				break;
			case '%':
				_extractEntity(buffer);
				break;
			case '<':
				_extractOverload(buffer);
				break;
			case '*':
				_extractBacktrack(buffer);
				break;
			case '{':
				_openSubbranchCapture();
				break;
			case '}':
				_closeSubbranchCapture();
				break;
			case '(':
				_extractNameOverload(buffer);
				break;
			default:
				break;
			}
		}
		if (cin.atEnd())
		{
			if (currentToAdd->type != independent_nodes::NotANode)
				currentNode->emplaceNode(currentToAdd);
			break;
		}
	}
	if (!root->isEmpty())
		root->type = independent_nodes::Subbranch;
	return root;
}

BranchDescriptionParser* BranchDescriptionParser::_instanse = Q_NULLPTR;
BranchDescriptionParser* BranchDescriptionParser::instanse()
{
	if (_instanse == Q_NULLPTR)
		_instanse = new BranchDescriptionParser();
	return _instanse;
}

BranchDescription BranchDescriptionParser::parse(QString what)
{
	return instanse()->doParsing(what);
}
