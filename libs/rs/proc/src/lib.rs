use proc_macro::TokenStream;
use quote::quote;
use syn::{parse_macro_input, DeriveInput, Data, Fields};

#[proc_macro_derive(ToPos)]
pub fn derive_to_pos(input: TokenStream) -> TokenStream
{
    let input = parse_macro_input!(input as DeriveInput);
    let name = &input.ident;

    match &input.data
    {
        Data::Struct(data_struct) =>
        {
            match &data_struct.fields
            {
                Fields::Named(fields) =>
                {
                    let has_x = fields.named.iter().any(|f| {
                        f.ident.as_ref().map_or(false, |i| i == "x")
                    });
                    let has_y = fields.named.iter().any(|f| {
                        f.ident.as_ref().map_or(false, |i| i == "y")
                    });
                    
                    if !has_x || !has_y
                    {
                        return syn::Error::new_spanned(
                            &input.ident,
                            "ToPos can only be derived for structs with 'x' and 'y' fields"
                        ).to_compile_error().into();
                    }
                }
                _ =>
                {
                    return syn::Error::new_spanned(
                        &input.ident,
                        "ToPos can only be derived for structs with named fields"
                    ).to_compile_error().into();
                }
            }
        }
        _ =>
        {
            return syn::Error::new_spanned(
                &input.ident,
                "ToPos can only be derived for structs"
            ).to_compile_error().into();
        }
    }

    let expanded = quote!
    {
        impl ::aoc::map::ToPos for #name
        {
            fn to_pos(&self) -> ::aoc::map::Pos
            {
                ::aoc::map::Pos
                {
                    y: self.y,
                    x: self.x,
                }
            }
        }
    };

    TokenStream::from(expanded)
}
